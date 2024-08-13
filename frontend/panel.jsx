import React from 'react'
import {Button, ButtonGroup} from "@nextui-org/button";
import ModContext from './modctx.jsx';
import { BsInfoSquare, BsShift, BsArrowReturnLeft, BsBackspace } from "react-icons/bs";

function InfoBtn(props) {
  return (
    <>
      <div className={"p-[0.1rem] w-full text-center " + props.className || ""}>
        <Button onPress={window.open_info_modal} size="sm" fullWidth className="min-w-0 text-xs"><BsInfoSquare /></Button>
      </div>
    </>
  );
}

function CtrlKeyBtn(props) {
  const mod = React.useContext(ModContext);
  function toggle() {
    if (mod.ctrlState == 0) {
      mod.setCtrlState(1);
    } else {
      mod.setCtrlState(0);
    }
  }
  return (
    <>
      <div className={"p-[0.1rem] w-full text-center " + props.className || ""}>
        <Button onPress={toggle} size="sm" fullWidth className="min-w-0 text-xs" color={ mod.ctrlState ? "primary" : "default" }>{props.children}</Button>
      </div>
    </>
  );
}

function AltKeyBtn(props) {
  const mod = React.useContext(ModContext);
  function toggle() {
    if (mod.altState == 0) {
      mod.setAltState(1);
    } else {
      mod.setAltState(0);
    }
  }
  return (
    <>
      <div className={"p-[0.1rem] w-full text-center " + props.className || ""}>
        <Button onPress={toggle} size="sm" fullWidth className="min-w-0 text-xs" color={ mod.altState ? "primary" : "default" }>{props.children}</Button>
      </div>
    </>
  );
}

function ShiftKeyBtn(props) {
  const mod = React.useContext(ModContext);
  function toggle() {
    if (mod.shiftState == 0) {
      mod.setShiftState(1);
    } else {
      mod.setShiftState(0);
    }
  }
  return (
    <>
      <div className={"p-[0.1rem] w-full text-center " + props.className || ""}>
        <Button onPress={toggle} size="sm" fullWidth className="min-w-0 text-xs" color={ mod.shiftState ? "primary" : "default" }>{props.children}</Button>
      </div>
    </>
  );
}

function KeyBtn(props) {
  const mod = React.useContext(ModContext);
  const keyopts = {
    code: props.evCode || 0,
    key: props.evKey != undefined ? props.evKey : (props.evCode || 0),
    charCode: props.evCharCode || 0,
    keyCode: props.evKeyCode || 0,
    which: props.evKeyCode || 0,
    view: window,
    shiftKey: props.evShiftKey || false,
  };
  function trigger() {
    if (window.send_event_to_term != null) {
      let fullopts = {
        ...keyopts,
        ctrlKey: mod.ctrlState,
        altKey: mod.altState,
      };
      if (props.evKeyCode != undefined) {
        window.send_event_to_term(new KeyboardEvent('keydown', {...fullopts, charCode: 0}));
      }
      if (props.evCharCode != undefined) {
        window.send_event_to_term(new KeyboardEvent('keypress', {...fullopts, keyCode: 0}));
      }
      mod.setCtrlState(0);
      mod.setAltState(0);
      // window.send_event_to_term(new KeyboardEvent('keyup', keyopts));
    }
  }
  return (
    <>
      <div className={"p-[0.1rem] w-full text-center " + props.className || ""}>
        <Button onPress={trigger} size="sm" fullWidth className="min-w-0 text-xs">{props.children}</Button>
      </div>
    </>
  );
}

function IfNotShift(props) {
  const mod = React.useContext(ModContext);
  return (
    <>
      {(!mod.shiftState) ? props.children : <></>}
    </>
  );
}

function IfShift(props) {
  const mod = React.useContext(ModContext);
  return (
    <>
      {mod.shiftState ? props.children : <></>}
    </>
  );
}

function LetterKeyBtn(props) {
  const mod = React.useContext(ModContext);
  const upper_letter = props.letter.toUpperCase();
  const lower_letter = props.letter.toLowerCase();
  const upper_char_code = upper_letter.charCodeAt(0);
  const lower_char_code = lower_letter.charCodeAt(0);
  if (mod.shiftState) {
    return (
      <>
        <KeyBtn evCode={"Key" + upper_letter} evKey={upper_letter} evCharCode={upper_char_code} evKeyCode={upper_char_code} className={props.className} evShiftKey>{upper_letter}</KeyBtn>
      </>
    );
  } else {
    return (
      <>
        <KeyBtn evCode={"Key" + upper_letter} evKey={lower_letter} evKeyCode={upper_char_code} className={props.className}>{lower_letter}</KeyBtn>
      </>
    );
  }
}

function Panel() {
  return (
    <>
      <div className="flex flex-col bg-background px-1 py-1">
        <div className="grid justify-items-center grid-cols-10">
          <KeyBtn evCode="Escape" evKeyCode={27} className="col-span-2">ESC</KeyBtn>
          <KeyBtn evCode="Tab" evKeyCode={9}>TAB</KeyBtn>
          <span className="col-span-4"></span>
          <InfoBtn />
          <KeyBtn evCode="Backspace" evKeyCode={8} className="col-span-2"><BsBackspace /></KeyBtn>
        </div>
        <IfNotShift>
          <div className="grid justify-items-center grid-cols-10">
            <KeyBtn evCode="Backquote" evKey="`" evKeyCode={192}>&#96;</KeyBtn>
            <KeyBtn evCode="BracketLeft" evKey="[" evKeyCode={219}>[</KeyBtn>
            <KeyBtn evCode="BracketRight" evKey="]" evKeyCode={221}>]</KeyBtn>
            <KeyBtn evCode="Backslash" evKey={"\\"} evKeyCode={220}>\</KeyBtn>
            <KeyBtn evCode="Semicolon" evKey=";" evKeyCode={59}>;</KeyBtn>
            <KeyBtn evCode="Semicolon" evKey=":" evKeyCode={59} evShiftKey>:</KeyBtn>
            <KeyBtn evCode="Quote" evKey="'" evKeyCode={222}>&#39;</KeyBtn>
            <KeyBtn evCode="Minus" evKey="-" evKeyCode={173}>-</KeyBtn>
            <KeyBtn evCode="Equal" evKey="=" evKeyCode={61}>=</KeyBtn>
            <KeyBtn evCode="Slash" evKey="/" evKeyCode={191}>/</KeyBtn>
          </div>
          <div className="grid justify-items-center grid-cols-10">
            <KeyBtn evCode="Digit1" evKey="1" evKeyCode={49}>1</KeyBtn>
            <KeyBtn evCode="Digit2" evKey="2" evKeyCode={50}>2</KeyBtn>
            <KeyBtn evCode="Digit3" evKey="3" evKeyCode={51}>3</KeyBtn>
            <KeyBtn evCode="Digit4" evKey="4" evKeyCode={52}>4</KeyBtn>
            <KeyBtn evCode="Digit5" evKey="5" evKeyCode={53}>5</KeyBtn>
            <KeyBtn evCode="Digit6" evKey="6" evKeyCode={54}>6</KeyBtn>
            <KeyBtn evCode="Digit7" evKey="7" evKeyCode={55}>7</KeyBtn>
            <KeyBtn evCode="Digit8" evKey="8" evKeyCode={56}>8</KeyBtn>
            <KeyBtn evCode="Digit9" evKey="9" evKeyCode={57}>9</KeyBtn>
            <KeyBtn evCode="Digit0" evKey="0" evKeyCode={48}>0</KeyBtn>
          </div>
        </IfNotShift>
        <IfShift>
          <div className="grid justify-items-center grid-cols-10">
            <KeyBtn evCode="Backquote" evKey="~" evKeyCode={192} evShiftKey>~</KeyBtn>
            <KeyBtn evCode="BracketLeft" evKey="{" evKeyCode={219} evShiftKey>&#123;</KeyBtn>
            <KeyBtn evCode="BracketRight" evKey="}" evKeyCode={221} evShiftKey>&#125;</KeyBtn>
            <KeyBtn evCode="Backslash" evKey="|" evKeyCode={220} evShiftKey>|</KeyBtn>
            <KeyBtn evCode="Semicolon" evKey=";" evKeyCode={59}>;</KeyBtn>
            <KeyBtn evCode="Semicolon" evKey=":" evKeyCode={59} evShiftKey>:</KeyBtn>
            <KeyBtn evCode="Quote" evKey={"\""} evKeyCode={222} evShiftKey>&#34;</KeyBtn>
            <KeyBtn evCode="Minus" evKey="_" evKeyCode={173} evShiftKey>_</KeyBtn>
            <KeyBtn evCode="Equal" evKey="+" evKeyCode={61} evShiftKey>+</KeyBtn>
            <KeyBtn evCode="Slash" evKey="?" evKeyCode={191} evShiftKey>?</KeyBtn>
          </div>
          <div className="grid justify-items-center grid-cols-10">
            <KeyBtn evCode="Digit1" evKey="!" evKeyCode={49} evShiftKey>!</KeyBtn>
            <KeyBtn evCode="Digit2" evKey="@" evKeyCode={50} evShiftKey>@</KeyBtn>
            <KeyBtn evCode="Digit3" evKey="#" evKeyCode={51} evShiftKey>#</KeyBtn>
            <KeyBtn evCode="Digit4" evKey="$" evKeyCode={52} evShiftKey>$</KeyBtn>
            <KeyBtn evCode="Digit5" evKey="%" evKeyCode={53} evShiftKey>%</KeyBtn>
            <KeyBtn evCode="Digit6" evKey="^" evKeyCode={54} evShiftKey>^</KeyBtn>
            <KeyBtn evCode="Digit7" evKey="&" evKeyCode={55} evShiftKey>&</KeyBtn>
            <KeyBtn evCode="Digit8" evKey="*" evKeyCode={56} evShiftKey>*</KeyBtn>
            <KeyBtn evCode="Digit9" evKey="(" evKeyCode={57} evShiftKey>(</KeyBtn>
            <KeyBtn evCode="Digit0" evKey=")" evKeyCode={48} evShiftKey>)</KeyBtn>
          </div>
        </IfShift>
        <div className="grid justify-items-center grid-cols-10">
          <LetterKeyBtn letter="q" />
          <LetterKeyBtn letter="w" />
          <LetterKeyBtn letter="e" />
          <LetterKeyBtn letter="r" />
          <LetterKeyBtn letter="t" />
          <LetterKeyBtn letter="y" />
          <LetterKeyBtn letter="u" />
          <LetterKeyBtn letter="i" />
          <LetterKeyBtn letter="o" />
          <LetterKeyBtn letter="p" />
        </div>
        <div className="grid justify-items-center grid-cols-[minmax(0,1fr)_repeat(9,minmax(0,2fr))_minmax(0,1fr)]">
          <span className="col-span-1"></span>
          <LetterKeyBtn letter="a" />
          <LetterKeyBtn letter="s" />
          <LetterKeyBtn letter="d" />
          <LetterKeyBtn letter="f" />
          <LetterKeyBtn letter="g" />
          <LetterKeyBtn letter="h" />
          <LetterKeyBtn letter="j" />
          <LetterKeyBtn letter="k" />
          <LetterKeyBtn letter="l" />
        </div>
        <div className="grid justify-items-center grid-cols-10">
          <ShiftKeyBtn><BsShift /></ShiftKeyBtn>
          <LetterKeyBtn letter="z" />
          <LetterKeyBtn letter="x" />
          <LetterKeyBtn letter="c" />
          <LetterKeyBtn letter="v" />
          <LetterKeyBtn letter="b" />
          <LetterKeyBtn letter="n" />
          <LetterKeyBtn letter="m" />
          <IfNotShift>
            <KeyBtn evCode="Comma" evKey="," evKeyCode={188}>,</KeyBtn>
            <KeyBtn evCode="Period" evKey="." evKeyCode={190}>.</KeyBtn>
          </IfNotShift>
          <IfShift>
            <KeyBtn evCode="Comma" evKey="<" evKeyCode={188} evShiftKey>&lt;</KeyBtn>
            <KeyBtn evCode="Period" evKey=">" evKeyCode={190} evShiftKey>&gt;</KeyBtn>
          </IfShift>
        </div>
        <div className="grid justify-items-center grid-cols-10">
          <CtrlKeyBtn>CTRL</CtrlKeyBtn>
          <AltKeyBtn>ALT</AltKeyBtn>
          <KeyBtn evCode="Space" evKey=" " evKeyCode={32} evCharCode={32} className="col-span-6">SPACE</KeyBtn>
          <KeyBtn evCode="Enter" evKeyCode={13} className="col-span-2"><BsArrowReturnLeft /></KeyBtn>
        </div>
      </div>
    </>
  )
}

export default Panel;

