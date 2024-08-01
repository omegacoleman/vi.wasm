import { Terminal } from '@xterm/xterm';
import '@xterm/xterm/css/xterm.css';
import wasmUrl from './vis.wasm?url';

var ttybuf = "";
var term = new Terminal({rows:24, cols:80, convertEol: true});
term.open(document.getElementById('terminal'));

var g_obj = null;
var g_instance = null;
var g_exports = null;
var g_memory = null;
var g_buffer = null;

var sleeping = false;

var resolve_enter = null;
var reject_enter = null;

var timeout_timer = null;

function enter() {
  return new Promise((resolve, reject) => {
    resolve_enter = resolve;
    reject_enter = reject;
    g_exports.entrance();
  });
}

function reenter() {
  if (!sleeping) return;
  try {
    let sbaddr = g_exports.asyncify_stackbuf_ptr();
    g_exports.asyncify_start_rewind(sbaddr);
    g_exports.entrance();
  } catch (error) {
    reject_enter(error);
  }
}

function yield_after_return() {
  let sbaddr = g_exports.asyncify_stackbuf_ptr();
  g_exports.asyncify_start_unwind(sbaddr);
}

term.onData(
  (str) => {
    ttybuf += str;
    if (timeout_timer != null) {
      clearTimeout(timeout_timer);
      timeout_timer = null;
    }
    reenter();
  }
);

async function run() {
  try {
    await enter();
  } catch (error) {
    term.write('\r\n\x1B[1;3;31m' + error + '\x1B[0m');
  }
}

WebAssembly.instantiateStreaming(fetch(wasmUrl), {
  wnglue: {
    ttyread: (buffer_start, buffer_end) => {
      if (sleeping) {
        g_exports.asyncify_stop_rewind();
        sleeping = false;
      }
      if (ttybuf.length != 0) {
        let view = new Uint8Array(g_buffer);
        let arr = view.subarray(buffer_start, buffer_end);
        let enc = new TextEncoder("utf-8");
        let result = enc.encodeInto(ttybuf, arr);
        ttybuf = ttybuf.slice(result.read);
        return buffer_start + result.written;
      }
      sleeping = true;
      yield_after_return();
      return buffer_start;
    },
    ttywait: (timeout_ms) => {
      if (sleeping) {
        g_exports.asyncify_stop_rewind();
        sleeping = false;

        if (ttybuf.length != 0) return 1;
        return 0;
      }

      if (ttybuf.length != 0) return 1;
      sleeping = true;
      yield_after_return();
      if (timeout_ms > 0) {
        timeout_timer = setTimeout(
          () => {
            timeout_timer = null;
            reenter();
          }, timeout_ms
        );
      }
      return 0;
    },
    ttywrite: (buffer_start, buffer_end) => {
      let dec = new TextDecoder("utf-8");
      let arr = new Uint8Array(g_buffer.slice(buffer_start, buffer_end));
      let str = dec.decode(arr);
      console.log(str);
      term.write(str);
      return buffer_end;
    },
    ttysize: (waddr, haddr) => {
      let view = new Uint32Array(g_buffer);
      view[waddr >> 2] = 80;
      view[haddr >> 2] = 24;
    },
    exit: (stcode) => {
      throw new Error("program exited with status " + stcode);
    },
    growmem: (incr) => {
      let sz_before = g_buffer.byteLength;
      let sz_after = sz_before + incr;
      let pg_before = Math.ceil(sz_before / (64 * 1024));
      let pg_after = Math.ceil(sz_after / (64 * 1024));
      if (pg_after > pg_before) {
        g_memory.grow(pg_after - pg_before);
        g_buffer = g_memory.buffer;
        console.log('growth=' + (pg_after - pg_before));
      }
      console.log('before: ' + sz_before + ' after:' + sz_after);
      return sz_before;
    },
    dbglog: (buffer_start, buffer_end) => {
      let dec = new TextDecoder("utf-8");
      let arr = new Uint8Array(g_buffer.slice(buffer_start, buffer_end));
      let str = dec.decode(arr);
      console.log(str);
      return buffer_end;
    },
  },
  }).then(
  (obj) => {
    g_obj = obj;
    g_instance = obj.instance;
    g_exports = g_instance.exports;
    g_memory = g_exports.memory;
    g_buffer = g_memory.buffer;
    run();
  },
);

