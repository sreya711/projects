import React, { Component } from "react";
import ReactPlayer from "react-player";
import Modal from "react-responsive-modal";

class Player extends Component {
  render() {
    const { open, toggleModal, sourceURL } = this.props;
    return (
      <Modal
        open={open}
        onClose={toggleModal}
        styles={{
          modal: {
            maxWidth: "unset",
            width: "100%",
            padding: "unset",
          },
          overlay: {
            background: "rgba(0, 0, 0, 0.5)"
          },
          closeButton: {
            background: "yellow"
          }
        }}
        center
      >
        {/* <video autoPlay controls muted width="90%" height="80%">
          <source src={sourceURL} type="video/mp4"></source>
        </video> */}
        <ReactPlayer playing
          url={sourceURL}
          width="100%"
          height="calc(100vh - 100px)"
          controls
          config={{ file: { attributes: { controlsList: 'nodownload' } } }}
        />
      </Modal>
    );
  }
}

export default Player;
