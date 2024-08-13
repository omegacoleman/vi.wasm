import React from 'react'
import { createRoot } from 'react-dom/client'
import {Helmet} from "react-helmet";
import {NextUIProvider} from '@nextui-org/react'
import Panel from './panel.jsx'
import DownloadModal from './downloadmodal.jsx'
import InfoModal from './infomodal.jsx'
import './index.css'
import mainjs from './main.js'
import { useDisclosure } from "@nextui-org/modal";
import ModContext from './modctx.jsx'

window.open_download_modal = null;

function RunMain() {
  React.useEffect(() => {mainjs();});
  return (<></>);
}

function App() {
  const [ctrlState, setCtrlState] = React.useState(0);
  const [altState, setAltState] = React.useState(0);
  const [shiftState, setShiftState] = React.useState(0);
  window.virtual_ctrl_state = ctrlState;
  window.set_virtual_ctrl_state = setCtrlState;
  window.virtual_alt_state = altState;
  window.set_virtual_alt_state = setAltState;

  const download_disclosure = useDisclosure();
  window.open_download_modal = download_disclosure.onOpen;

  const info_disclosure = useDisclosure({ defaultOpen: true });
  window.open_info_modal = info_disclosure.onOpen;

  return (
    <div className="fixed flex bg-background w-dvw h-dvh flex-col-reverse flex-nowrap">
      <ModContext.Provider value={{
        ctrlState,
        setCtrlState,
        altState,
        setAltState,
        shiftState,
        setShiftState,
      }}>
        <Panel />
        <DownloadModal isOpen={download_disclosure.isOpen} onOpenChange={download_disclosure.onOpenChange} />
        <InfoModal isOpen={info_disclosure.isOpen} onOpenChange={info_disclosure.onOpenChange} />
        <div id="terminal-wrapper" className="grow min-h-0">
        <div id="terminal" className="w-full h-full">
        <RunMain />
        </div>
        </div>
      </ModContext.Provider>
    </div>
  );
}

createRoot(document.getElementById('root')).render(
    <NextUIProvider>
      <main className="dark text-foreground bg-background">
        <App />
      </main>
    </NextUIProvider>
  ,
)

