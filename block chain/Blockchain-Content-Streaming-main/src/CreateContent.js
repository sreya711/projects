import { useState } from 'react'
import { ethers } from "ethers"
import { Row, Form, Button } from 'react-bootstrap'
import { create as ipfsHttpClient } from 'ipfs-http-client'
import { Buffer } from 'buffer';

const projectId = '2I83he1ZXBXLTicn65OG1QWOBh1';
const projectSecret = 'e73b1022efdc918442f364b945388b6e';
const auth =
  'Basic ' + Buffer.from(projectId + ':' + projectSecret).toString('base64');

const client = ipfsHttpClient({
  host: 'ipfs.infura.io',
  port: 5001,
  protocol: 'https',
  headers: {
    authorization: auth,
  },
});

const CreateContent = ({ contract }) => {
  const [image, setImage] = useState('')
  const [video, setVideo] = useState('')
  const [price, setPrice] = useState(null)
  const [name, setName] = useState('')
  const [description, setDescription] = useState('')
  const [imageUploaded, setImageUploaded] = useState('')
  const [videoUploaded, setVideoUploaded] = useState('')
  const [createContentAccess, setCreateContentAccess] = useState('')

  const uploadImageToIPFS = async (event) => {
    event.preventDefault()
    const file = event.target.files[0]
    if (typeof file !== 'undefined') {
      try {
        const result = await client.add(file)
        console.log("Image uploaded to IPFS")
        setImage(`https://ipfs.io/ipfs/${result.path}`)
        setImageUploaded('Image Uploaded to IPFS!')
      } catch (error) {
        console.log("ipfs image upload error: ", error)
      }
    }
  }

  const uploadVideoToIPFS = async (event) => {
    event.preventDefault()
    const file = event.target.files[0]
    if (typeof file !== 'undefined') {
      try {
        const result = await client.add(file)
        console.log("Video uploaded to IPFS")
        setVideo(`https://ipfs.io/ipfs/${result.path}`)
        setVideoUploaded('Video Uploaded to IPFS!')
      } catch (error) {
        console.log("ipfs image upload error: ", error)
      }
    }
  }

  const createContent = async (event) => {
    if (!video || !image || !price || !name || !description) {
      console.log('Add all the data')
      setCreateContentAccess('Add all the data')
      return
    }
    if (!video && !image) {
      console.log('Let the video and image upload to IPFS')
      setCreateContentAccess('Let the video and image upload to IPFS')
      return
    }
    if (!image) {
      console.log('Let the image upload to IPFS')
      setCreateContentAccess('Let the image upload to IPFS')
      return
    }
    if (!video) {
      console.log('Let the video upload to IPFS')
      setCreateContentAccess('Let the video upload to IPFS')
      return
    }
    setCreateContentAccess('')
    var result;
    try {
      result = await client.add(JSON.stringify({ image, video, price, name, description }))
      console.log(result);
    } catch (error) {
      console.log("ipfs uri upload error: ", error)
    }

    const uri = `https://ipfs.io/ipfs/${result.path}`
    console.log(uri);
    console.log("URI Set");

    const id = await (await contract.addFile(uri, ethers.utils.parseEther(price.toString()))).wait();
    console.log("File Added");
    console.log(id);
    const contentId = await contract.contentId();
    console.log("Content ID fetched");
    console.log(contentId);
    setCreateContentAccess('Content Added!')
    // console.log(ethers.utils.formatUnits(id, 0));
  }

  return (
    <div className="container-fluid mt-4 mb-4">
      <div className="row">
        <main role="main" className="col-lg-12 mx-auto" style={{ maxWidth: '1000px' }}>
          <div className="content mx-auto">
            <Row className="g-4">
              <Form.Group controlId="formFile">
                <Form.Label>Choose the Thumbnail File</Form.Label>
                <Form.Control
                  type="file"
                  required
                  name="file"
                  onChange={uploadImageToIPFS}
                  title="Choose Image File"
                />
                {imageUploaded != '' ? <p style={{ color: '#00FF00' }}>{imageUploaded}</p> : <p></p>}
              </Form.Group>              
              <Form.Group controlId="formFile">
                <Form.Label>Choose the Video File</Form.Label>
                <Form.Control
                  type="file"
                  required
                  name="file"
                  onChange={uploadVideoToIPFS}
                  title="Choose Video File"
                />
                {videoUploaded != '' ? <p style={{ color: '#00FF00' }}>{videoUploaded}</p> : <p></p>}
              </Form.Group>
              <Form.Group controlId="formFile">
                <Form.Control onChange={(e) => setName(e.target.value)} size="lg" required type="text" placeholder="Name" />
              </Form.Group>
              <Form.Group controlId="formFile">
                <Form.Control onChange={(e) => setDescription(e.target.value)} size="lg" required as="textarea" placeholder="Description" />
              </Form.Group>
              <Form.Group controlId="formFile">
                <Form.Control onChange={(e) => setPrice(e.target.value)} size="lg" required type="number" placeholder="Price in ETH" />
              </Form.Group>
              <div className="d-grid px-0">
                <Button onClick={createContent} variant="primary" size="lg">
                  Create Content!
                </Button>
                {createContentAccess != '' && createContentAccess != 'Content Added!' ? <p style={{ color: '#FF0000' }}>{createContentAccess}</p> : <p></p>}
                {createContentAccess == 'Content Added!' ? <h4 style={{ color: '#00FF00'}}>{createContentAccess}</h4> : <p></p>}
              </div>
              {/* <video controls muted >
                <source src={`https://infura-ipfs.io/ipfs/QmUFayzpxCiKMooozYdEP6McKZjHnSgPdBQDphXYZ33THx`} type="video/mp4"></source>
              </video> */}
            </Row>
          </div>
        </main>
      </div>
    </div>
  );
}

export default CreateContent