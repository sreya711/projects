import { useState, useEffect } from 'react'
import { ethers } from "ethers"
import { Row, Col, Card, Button, Container } from 'react-bootstrap'
import Player from './ReactPlayer';

const AllContent = ({ contract, account }) => {
  const [loading, setLoading] = useState(true)
  const [contents, setContents] = useState([])
  const [open, setOpen] = useState(false)
  const [sourceURL, setSourceURL] = useState('')

  const onOpenModal = (content) => {
    setSourceURL(content.video);
    setOpen(!open);
    loadAllContents();
  };

  const payPrice = async (content) => {
    let response;
    if (content.owner.toLowerCase() != account) {
      response = await contract.payPrice(content.contentId, { value: ethers.utils.parseEther(content.price.toString()) });
    }
    onOpenModal(content);
  }

  const loadAllContents = async () => {
    console.log(account);
    // Load all contents
    const contentCount = await contract.contentId();
    let contents = []
    for (let i = 1; i <= contentCount; i++) {
      const content = await contract.contents(i)
      console.log(content);

      const contractUsersWithAccess = await contract.getUserWithAccess(i);
      console.log(contractUsersWithAccess);

      const usersWithAccess = await contractUsersWithAccess?.map(element => {
        console.log(element.toLowerCase())
        return element.toLowerCase();
      });

      // get uri url from nft contract
      const uri = await content.fileLocation

      // use uri to fetch the nft metadata stored on ipfs 
      const response = await fetch(uri)
      const metadata = await response.json()

      // Add content to contents array
      contents.push({
        price: metadata.price,
        contentId: i,
        owner: content.owner,
        name: metadata.name,
        description: metadata.description,
        image: metadata.image,
        video: metadata.video,
        usersWithAccess: usersWithAccess
      })
    }
    setLoading(false)
    setContents(contents)
  }

  useEffect(() => {
    loadAllContents()
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
                      {content.owner.toLowerCase() == account || content.usersWithAccess?.includes(account) ? <Button onClick={() => payPrice(content)} variant="primary" size="lg">
                        Watch
                      </Button> :
                        <Button onClick={() => payPrice(content)} variant="primary" size="lg">
                          Watch for {content.price} ETH!
                        </Button>}
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
        ) : <Player open={open} toggleModal={onOpenModal} sourceURL={sourceURL} />}
    </div>
  );
}

export default AllContent