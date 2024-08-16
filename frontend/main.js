import { Terminal } from '@xterm/xterm';
import { FitAddon } from '@xterm/addon-fit';
import '@xterm/xterm/css/xterm.css';
import wasmUrl from './vis.wasm?url';

export default function mainjs() {

var ttybuf = "";
var term = new Terminal({convertEol: true});
var fitAddon = new FitAddon();
term.loadAddon(fitAddon);

var term_elem = document.getElementById('terminal');
term.open(term_elem);
fitAddon.fit();

window.send_event_to_term = function (ev) {
  term.textarea.dispatchEvent(ev);
}

var last_down_is_ctrl = false;
term.attachCustomKeyEventHandler(ev => {
  if (window.virtual_ctrl_state == undefined) return true;
  let stateMod = {};
  let doMod = false;
  if (window.virtual_ctrl_state != 0 && (!ev.ctrlKey)) {
    stateMod.ctrlKey = true;
    doMod = true;
  }
  if (window.virtual_alt_state != 0 && (!ev.altKey)) {
    stateMod.altKey = true;
    doMod = true;
  }
  if (doMod) {
    let mev = new KeyboardEvent(ev.type, {
      code: ev.code,
      key: ev.key,
      keyCode: ev.keyCode,
      charCode: ev.charCode,
      which: ev.which,
      view: ev.view,
      shiftKey: ev.shiftKey,
      ctrlKey: ev.ctrlKey,
      altKey: ev.altKey,
      ...stateMod,
    });
    term.textarea.dispatchEvent(mev);
    // console.log(mev);
    return false;
  }
  if (ev.type === 'keyup' && ev.key !== 'Control' && ev.key !== 'Alt' && ev.key !== 'Shift') {
    window.set_virtual_ctrl_state(0);
    window.set_virtual_alt_state(0);
  }
  if (ev.type === 'keydown') {
    if (ev.key === 'Control') {
      last_down_is_ctrl = true;
    } else {
      last_down_is_ctrl = false;
    }
  }
  if (ev.type === 'keypress') {
    last_down_is_ctrl = false;
  }
  if (ev.type === 'keyup' && ev.key === 'Control' && last_down_is_ctrl) {
    if (window.virtual_ctrl_state == 0) {
      window.set_virtual_ctrl_state(1);
    } else {
      window.set_virtual_ctrl_state(0);
    }
  }
});

// term.onKey((ev) => { console.log(ev); })

var g_obj = null;
var g_instance = null;
var g_exports = null;
var g_memory = null;
var g_buffer = null;

var sleeping = false;

var resolve_enter = null;
var reject_enter = null;

var timeout_timer = null;

var ewinch = -4;
var is_winch = false;

var kv = {};

function enter() {
  return new Promise((resolve, reject) => {
    resolve_enter = resolve;
    reject_enter = reject;
    g_exports.entrance();
  });
}

function reenter() {
  if (!sleeping) return;
  if (timeout_timer != null) {
    clearTimeout(timeout_timer);
    timeout_timer = null;
  }
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
    reenter();
  }
);

const resizeObserver = new ResizeObserver(
  (entries) => {
    fitAddon.fit();
    if (!g_exports || !sleeping) return;
    is_winch = true;
    reenter();
  }
);

resizeObserver.observe(document.getElementById('terminal-wrapper'));

async function run() {
  try {
    await enter();
  } catch (error) {
    term.write('\r\n\x1B[1;3;31m' + error + '\x1B[0m');
  }
}

function extract_u8arr(start, end) {
  return new Uint8Array(g_buffer.slice(start, end));
}

function extract_str(start, end) {
  let dec = new TextDecoder("utf-8");
  return dec.decode(extract_u8arr(start, end));
}

WebAssembly.instantiateStreaming(fetch(wasmUrl), {
  wnglue: {
    ttyread: (buffer_start, buffer_end) => {
      if (sleeping) {
        g_exports.asyncify_stop_rewind();
        sleeping = false;
        if (is_winch) {
          is_winch = false;
          return ewinch;
        }
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
        if (is_winch) {
          is_winch = false;
          if (ttybuf.length == 0) return ewinch;
        }

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
      term.write(str);
      return buffer_end;
    },
    ttysize: (waddr, haddr) => {
      let view = new Uint32Array(g_buffer);
      view[waddr >> 2] = term.cols;
      view[haddr >> 2] = term.rows;
      // console.log("cols=" + term.cols + " rows=" + term.rows);
    },
    kvpread: (key_start, key_end, pos, buffer_start, buffer_end) => {
      console.log("kvpread");
      let key = extract_str(key_start, key_end);
      console.log(str);
      return buffer_start;
    },
    kvpwrite: (key_start, key_end, pos, buffer_start, buffer_end) => {
      console.log("kvpwrite");
      let key = extract_str(key_start, key_end);
      return buffer_end;
    },
    kvlen: (key_start, key_end) => {
      console.log("kvlen");
      return 0;
    },
    kvsetlen: (key_start, key_end, len) => {
      console.log("kvsetlen");
      return 0;
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
        // console.log('growth=' + (pg_after - pg_before));
      }
      // console.log('before: ' + sz_before + ' after:' + sz_after);
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

}

