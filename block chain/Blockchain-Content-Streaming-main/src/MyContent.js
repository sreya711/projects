import { useState, useEffect } from 'react'
import { Row, Col, Card, Button, Container } from 'react-bootstrap'
import 'reactjs-popup/dist/index.css';
import Player from './ReactPlayer';


const MyContent = ({ contract, account }) => {
  const [loading, setLoading] = useState(true)
  const [contents, setContents] = useState([])
  const [open, setOpen] = useState(false)
  const [sourceURL, setSourceURL] = useState('')

  const onOpenModal = (content) => {
    setSourceURL(content.video);
    setOpen(!open);
  };

  const loadMyContents = async () => {
    // Load all My contents
    console.log("account" + account);
    const contentCount = await contract.contentId();
    const usersAddedContents = await contract.getUsersAddedContents();
    console.log(usersAddedContents);
    
    let contents = []
    for (let i = 1; i <= contentCount; i++) {
      console.log("for entered");
      const content = await contract.contents(i)
      // const usersWithAccess = await content.usersWithAccess?.map(element => {
      //   console.log(element.toLowerCase())
      //   return element.toLowerCase();
      // });
      console.log(content.owner.toLowerCase())
      if (content.owner.toLowerCase() == account) {
        console.log("entered");
        // get uri url from nft contract
        const uri = await content.fileLocation

        // use uri to fetch the nft metadata stored on ipfs 
        const response = await fetch(uri)
        const metadata = await response.json()
        console.log(metadata);

        // Add content to contents array
        contents.push({
          price: metadata.price,
          contentId: i,
          owner: content.owner,
          name: metadata.name,
          description: metadata.description,
          image: metadata.image,
          video: metadata.video,
        })
      }
    }
    setLoading(false)
    setContents(contents)
    console.log(contents.length);
  }

  useEffect(() => {
    loadMyContents()
  }, [])
  if (loading) return (
    <main style={{ padding: "1rem 0" }}>
      <h2>Loading...</h2>
    </main>
  )

  return (
    <div className="flex justify-center">
      {!open ? contents.length > 0 ?
        <div className="px-5 container">
          <Row xs={1} md={2} lg={4} className="g-4 py-5">
            {contents.map((content, idx) => (
              <Col key={idx} className="overflow-hidden">
                <Card>
                  <Card.Img variant="top" src={content.image} />
                  <Card.Body color="secondary">
                    <Card.Title>{content.name}</Card.Title>
                    <Card.Text>
                      {content.description}
                    </Card.Text>
                  </Card.Body>
                  <Card.Footer>
                    <div className='d-grid'>
                      <Button onClick={() => onOpenModal(content)} variant="primary" size="lg">
                        Watch
                      </Button>
                    </div>
                  </Card.Footer>
                </Card>
              </Col>
            ))}
          </Row>
        </div>
        : (
          <main style={{ padding: "1rem 0" }}>
            <h2>No listed assets</h2>
          </main>
        )
        : <Player open={open} toggleModal={onOpenModal} sourceURL={sourceURL} />}
    </div>
  );
}

export default MyContent
