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
import { BsFiletypeTxt } from "react-icons/bs";

function DownloadModal(props) {
  return (
  <>
    <Modal className="dark text-foreground" isOpen={props.isOpen} onOpenChange={props.onOpenChange}>
      <ModalContent>
        {(onClose) => (
          <>
            <ModalHeader className="flex flex-col gap-1">Download</ModalHeader>
            <ModalBody>
              <p>You have attempted to save following file to disk:</p>
              <Card className="max-w-[400px]">
                <CardFooter className="flex gap-3">
                  <div className="text-[3rem] w-[4rem] h-[4rem] grid place-content-center">
                    <BsFiletypeTxt />
                  </div>
                  <div className="flex flex-col">
                    <p className="text-md">hello.txt</p>
                    <p className="text-small text-default-500">20kB</p>
                  </div>
                </CardFooter>
              </Card>
              <p>Do you want to download it?</p>
            </ModalBody>
            <ModalFooter>
              <Button onPress={onClose}>
                Cancel
              </Button>
              <Button color="primary" onPress={onClose}>
                Download
              </Button>
            </ModalFooter>
          </>
        )}
      </ModalContent>
    </Modal>
  </>
  );
}

export default DownloadModal;

