import React from "react";
import {Button, ButtonGroup} from "@nextui-org/button";
import {
  Modal,
  ModalContent,
  ModalHeader,
  ModalBody,
  ModalFooter
} from "@nextui-org/modal";
import {Card, CardHeader, CardBody, CardFooter} from "@nextui-org/card";
import { BsGithub } from "react-icons/bs";

function InfoModal(props) {
  return (
  <>
    <Modal className="dark text-foreground" isOpen={props.isOpen} onOpenChange={props.onOpenChange}>
      <ModalContent>
        {(onClose) => (
          <>
            <ModalHeader className="flex flex-col gap-1">Hello there!</ModalHeader>
            <ModalBody>
              <p>Welcome to vi.wasm!</p>
              <p>This is a fork of vis stripped down to the core, compiled to wasm and running on a webpage.</p>
              <p className="text-center">
                <Button as="a" href="https://github.com/omegacoleman/vi.wasm/" target="_blank" className="bg-white text-black" size="md">
                  <BsGithub />
                  Github Repo
                </Button>
              </p>
              <p className="text-white/50">Hint: to input &lt;C-w&gt;, press CTRL separately then press W</p>
            </ModalBody>
            <ModalFooter>
              <Button color="primary" onPress={onClose}>
                Close
              </Button>
            </ModalFooter>
          </>
        )}
      </ModalContent>
    </Modal>
  </>
  );
}

export default InfoModal;

