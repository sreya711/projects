import { useState } from 'react';
import { Navbar, Nav, Button, Container, Row, Col, Form } from 'react-bootstrap';
import { ethers } from "ethers";
import './App.css';

import SupplyChainAbi from './backend/build/contracts/SupplyChain.json'

function App() {
  const [account, setAccount] = useState(null)
  const [supplyChain, setSupplyChain] = useState({})
  const [clientName, setClientName] = useState("")
  const [clientAddress, setClientAddress] = useState("")
  const [clientAddressFetched, setClientAddressFetched] = useState("")
  const [supplierName, setSupplierName] = useState("")
  const [supplierLimit, setSupplierLimit] = useState(null)
  const [supplierLimitAdded, setSupplierLimitAdded] = useState(null)
  const [supplierLimitFetched, setSupplierLimitFetched] = useState(null)
  const [bidderName, setBidderName] = useState("")
  const [bidderPrice, setBidderPrice] = useState(null)
  const [bidderQuantity, setBidderQuantity] = useState(null)
  const [bidsForVedantaAdded, setBidsForVedantaAdded] = useState("")
  const [bidsForMRFAdded, setBidsForMRFAdded] = useState("")
  const [bidsForCeatAdded, setBidsForCeatAdded] = useState("")
  const [bidsForVedantaFetched, setBidsForVedantaFetched] = useState("")
  const [bidsForMRFFetched, setBidsForMRFFetched] = useState("")
  const [bidsForCeatFetched, setBidsForCeatFetched] = useState("")
  const [isDistributed, setIsDistributed] = useState("")
  const [isManufactured, setIsManufactured] = useState("")
  const [manufacturerName, setManufacturerName] = useState("")
  const [buyerName, setBuyerName] = useState("")
  const [carsLimit, setCarsLimit] = useState("")
  const [carIdBought, setCarIdBought] = useState(null)
  const [customerName, setCustomerName] = useState(null)
  const [carIndexesBought, setCarIndexesBought] = useState([])
  const [carId, setCarId] = useState(null)
  const [carDetails, setCarDetails] = useState({})
  const [loading1, setLoading1] = useState(true)
  const [loading2, setLoading2] = useState(true)
  const [loading3, setLoading3] = useState(true)
  const [loading4, setLoading4] = useState(true)
  const [loading5, setLoading5] = useState(true)
  const [loading6, setLoading6] = useState(true)
  const [loading7, setLoading7] = useState(true)
  const [loading8, setLoading8] = useState(true)

  const supplyChainAddress = "0xA1b297A256752f2D079DDc6c2A78F6c1F1233e03"

  const web3Handler = async () => {
    const accounts = await window.ethereum.request({ method: 'eth_requestAccounts' });
    setAccount(accounts[0])
    // Get provider from Metamask
    const provider = new ethers.providers.Web3Provider(window.ethereum)
    // Set signer
    const signer = provider.getSigner()

    window.ethereum.on('chainChanged', (chainId) => {
      window.location.reload();
    })

    window.ethereum.on('accountsChanged', async function (accounts) {
      setAccount(accounts[0])
      await web3Handler()
    })
    loadContracts(signer)
  }

  const loadContracts = async (signer) => {
    // Get deployed copies of contracts
    const supplyChainContract = new ethers.Contract(supplyChainAddress, SupplyChainAbi.abi, signer)
    setSupplyChain(supplyChainContract)
  }

  const setClient = async () => {
    console.log(clientName);
    console.log(clientAddress);
    setLoading1(true);
    await (await supplyChain.setClient(clientName, clientAddress)).wait()
    console.log("Client Added");
    setLoading1(false);
  }

  const getClient = async () => {
    setClientAddressFetched("");
    const clientAddress = await supplyChain.getClient(clientName)
    console.log(clientAddress);
    setClientAddressFetched(clientAddress);
  }

  const setSupplierLimits = async () => {
    var supplierLimitAdded;
    setLoading2(true);
    try {
      await (await supplyChain.setSupplierLimit(supplierName, supplierLimit)).wait()
      supplierLimitAdded = "Supplier Limits Added"
    } catch (error) {
      supplierLimitAdded = "Only the particular supplier can set their limit"
    }
    setSupplierLimitAdded(supplierLimitAdded);
    console.log("Supplier Limits Added");
    setLoading2(false);
  }

  const getSupplierLimits = async () => {
    setSupplierLimitFetched(null);
    const supplierLimit = await supplyChain.getSupplierLimit(supplierName)
    console.log(supplierLimit);
    setSupplierLimitFetched(ethers.utils.formatUnits(supplierLimit, 0));
  }

  const setBidsForVedanta = async () => {
    var bidsForVedantaAdded;
    setLoading3(true);
    try {
      await (await supplyChain.setBidForVedanta(bidderName, bidderPrice, bidderQuantity)).wait()
      bidsForVedantaAdded = "Bid Added For Vedanta";
    } catch (error) {
      bidsForVedantaAdded = "Only the particular bidder can set their bid";
    }
    setBidsForVedantaAdded(bidsForVedantaAdded);
    setLoading3(false);
  }

  const setBidsForMRF = async () => {
    var bidsForMRFAdded;
    setLoading4(true);
    try {
      await (await supplyChain.setBidForMRF(bidderName, bidderPrice, bidderQuantity)).wait()
      bidsForMRFAdded = "Bid Added For MRF";
    } catch (error) {
      bidsForMRFAdded = "Only the particular bidder can set their bid";
    }
    setBidsForMRFAdded(bidsForMRFAdded);
    setLoading4(false);
  }

  const setBidsForCeat = async () => {
    var bidsForCeatAdded
    setLoading5(true);
    try {
      await (await supplyChain.setBidForCeat(bidderName, bidderPrice, bidderQuantity)).wait()
      bidsForCeatAdded = "Bid Added For Ceat";
    } catch (error) {
      bidsForCeatAdded = "Only the particular bidder can set their bid";
    }
    setBidsForCeatAdded(bidsForCeatAdded);
    setLoading5(false);
  }

  const getBidsForVedanta = async () => {
    var bidsFetched = "";
    setBidsForVedantaFetched("");
    try {
      const bidsForVedanta = await supplyChain.getVedantaBid(bidderName);
      bidsFetched = "Price: " + ethers.utils.formatUnits(bidsForVedanta.price.toString(), 0).toString() + "\n, Quantity: " + ethers.utils.formatUnits(bidsForVedanta.quantity.toString(), 0).toString();
      console.log(bidsForVedanta);
    } catch (error) {
      bidsFetched = "The bidding is still in progess, Please wait !!";
      console.log(bidsFetched);
    }
    setBidsForVedantaFetched(bidsFetched);
    setBidderName("")
  }

  const getBidsForMRF = async () => {
    var bidsFetched = "";
    setBidsForMRFFetched("");
    try {
      const bidsForMRF = await supplyChain.getMRFBid(bidderName);
      bidsFetched = "Price: " + ethers.utils.formatUnits(bidsForMRF.price.toString(), 0).toString() + ", Quantity: " + ethers.utils.formatUnits(bidsForMRF.quantity.toString(), 0).toString();
      console.log(bidsForMRF);
    } catch (error) {
      bidsFetched = "The bidding is still in progess, Please wait !!";
      console.log(bidsFetched);
    }
    setBidsForMRFFetched(bidsFetched);
  }

  const getBidsForCeat = async () => {
    var bidsFetched = "";
    setBidsForCeatFetched("");
    try {
      const bidsForCeat = await supplyChain.getCeatBid(bidderName);
      bidsFetched = "Price: " + ethers.utils.formatUnits(bidsForCeat.price.toString(), 0).toString() + ", Quantity: " + ethers.utils.formatUnits(bidsForCeat.quantity.toString(), 0).toString();
      console.log(bidsForCeat);
    } catch (error) {
      bidsFetched = "The bidding is still in progess, Please wait !!";
      console.log(bidsFetched);
    }
    setBidsForCeatFetched(bidsFetched);
  }

  const distributeOptimally = async () => {
    var distributed = "";
    setIsDistributed("");
    try {
      await (await supplyChain.distributeOptimally()).wait();
      distributed = "The Materials are distributed optimally";
    } catch (error) {
      distributed = "The bidding is still in progess, Please wait !!";
      console.log(distributed);
    }
    setIsDistributed(distributed);
  }

  const manufactureCars = async () => {
    var manufactured = "";
    setIsManufactured("");
    try {
      await (await supplyChain.manufactureCars()).wait();
      manufactured = "The Materials are manufactured optimally";
    } catch (error) {
      manufactured = "The bidding/distribution is still in progess, Please wait !!";
      console.log(error);
    }
    setIsManufactured(manufactured);
  }

  const getManufacturedCarsLimit = async () => {
    var limit;
    setCarsLimit("");
    try {
      limit = (await supplyChain.getManufacturedCarsLimit(manufacturerName)).toString();
    } catch (error) {
      limit = "The bidding/distribution/manufacturing is still in progess, Please wait !!";
      console.log(limit);
    }
    setCarsLimit(limit);
  }

  const buyCar = async () => {
    var carIdBought;
    var carIdBoughtObject;
    setCarIdBought("");
    try {
      carIdBoughtObject = await supplyChain.purchaseCar(buyerName, manufacturerName);
      console.log(ethers.utils.formatUnits(carIdBoughtObject.value, 0));
      carIdBought = ethers.utils.formatUnits(carIdBoughtObject.value, 0)
    } catch (error) {
      carIdBought = "The bidding/distribution/manufacturing is still in progess, Please wait !!";
      console.log(carIdBought);
    }
    setCarIdBought(carIdBought);
  }

  const getUserBoughtIndexes = async () => {
    var carIndexesBought = [];
    var carIndexesBoughtContract = [];
    setCarIndexesBought("");
    try {
      carIndexesBoughtContract = await supplyChain.checkCarIndexOwnedByCustomer(customerName);
      for (var i = 0; i < carIndexesBoughtContract.length; i++) {
        console.log(carIndexesBoughtContract[i])
        carIndexesBought.push(ethers.utils.formatUnits(carIndexesBoughtContract[i], 0));
        carIndexesBought.push(", ")
      }
      console.log(carIndexesBought)
    } catch (error) {
      carIndexesBought = [];
      console.log(carIndexesBought);
    }
    setCarIndexesBought(carIndexesBought);
  }

  const getCarDetails = async () => {
    let carDetails = {
      "Car": {
        "id": 0,
        "manufacturer_name": "",
        "Wheel": {
          "id": 0,
          "owner": "",
          "supplier_name": "",
          "price": 0,
        },
        "Car_body": {
          "id": 0,
          "owner": "",
          "supplier_name": "",
          "price": 0,
        },
        "price": 0,
        "sold": false,
      }
    };
    var carDetailsContract;
    setCarDetails({});
    try {
      carDetailsContract = await supplyChain.checkCarDetails(carId);
      console.log(carDetailsContract)
      carDetails["Car"]["id"] = ethers.utils.formatUnits(carDetailsContract["id"], 0)
      carDetails["Car"]["manufacturer_name"] = carDetailsContract["manufacturerName"]
      carDetails["Car"]["Wheel"]["id"] = ethers.utils.formatUnits(carDetailsContract["wheel"]["id"], 0)
      carDetails["Car"]["Wheel"]["owner"] = carDetailsContract["wheel"]["owner"]
      carDetails["Car"]["Wheel"]["supplier_name"] = carDetailsContract["wheel"]["supplierName"]
      carDetails["Car"]["Wheel"]["price"] = ethers.utils.formatUnits(carDetailsContract["wheel"]["price"], 0)
      carDetails["Car"]["Car_body"]["id"] = ethers.utils.formatUnits(carDetailsContract["carBody"]["id"], 0)
      carDetails["Car"]["Car_body"]["owner"] = carDetailsContract["carBody"]["owner"]
      carDetails["Car"]["Car_body"]["supplier_name"] = carDetailsContract["carBody"]["supplierName"]
      carDetails["Car"]["Car_body"]["price"] = ethers.utils.formatUnits(carDetailsContract["carBody"]["price"], 0)
      carDetails["Car"]["price"] = ethers.utils.formatUnits(carDetailsContract["price"], 0);
      carDetails["Car"]["sold"] = carDetailsContract["isOwned"];
    } catch (error) {
      carDetails = "The bidding/distribution/manufacturing is still in progess, Please wait !!";
      console.log(carDetails);
    }
    setCarDetails(carDetails);
  }

  return (
    <div className="App">
      <Navbar expand="lg" bg="secondary" variant="dark">
        <Container>
          <Navbar.Brand href="">
            &nbsp; Supply Chain Management (Team: RSA)
          </Navbar.Brand>
          <Navbar.Toggle aria-controls="responsive-navbar-nav" />
          <Navbar.Collapse id="responsive-navbar-nav">
            <Nav className="me-auto">
            </Nav>
            <Nav>
              {account ? (
                <Nav.Link
                  href={`https://etherscan.io/address/${account}`}
                  target="_blank"
                  rel="noopener noreferrer"
                  className="button nav-button btn-sm mx-4">
                  <Button variant="outline-light">
                    {account.slice(0, 5) + '...' + account.slice(38, 42)}
                  </Button>

                </Nav.Link>
              ) : (
                <Button onClick={web3Handler} variant="outline-light">Connect Wallet</Button>
              )}
            </Nav>
          </Navbar.Collapse>
        </Container>
      </Navbar>

      <br></br>

      {/* Set Clients */}
      <Container>
        <Row>
          <Col>
            <h4>Set Clients: </h4>
          </Col>
          <Container>
            <Col>
              <Container>
                <Row>
                  <Col>
                    <Form.Control onChange={(e) => setClientName(e.target.value)} size="md" required type="text" placeholder="Enter Client Name" />
                  </Col>
                  <Col>
                    <Form.Control onChange={(e) => setClientAddress(e.target.value)} size="md" required type="text" placeholder="Enter Client Wallet Address" />
                  </Col>
                </Row>
              </Container>
              <br></br>
              <Container>
                <Row>
                  <Col>
                    <div>
                      <Button onClick={setClient} variant="primary" size="md">
                        Set Client!
                      </Button>
                    </div>
                  </Col>
                  <Col>
                    {loading1 == false ? <h4>Client Added</h4> : <h4></h4>}
                  </Col>
                </Row>
              </Container>
            </Col>
          </Container>
        </Row>
      </Container>

      <br></br>

      {/* Get Clients */}
      <Container>
        <Row>
          <Col>
            <h4>Get Client Address: </h4>
          </Col>
          <Container>
            <Col>
              <Container>
                <Row>
                  <Col lg={6}>
                    <Form.Control onChange={(e) => setClientName(e.target.value)} size="md" required type="text" placeholder="Enter Client Name" />
                  </Col>
                </Row>
              </Container>
              <br></br>
              <Container>
                <Row>
                  <Col>
                    <div>
                      <Button onClick={getClient} variant="primary" size="md">
                        Get Client Address!
                      </Button>
                    </div>
                  </Col>
                  <Col>
                    <h4>{clientAddressFetched}</h4>
                  </Col>
                </Row>
              </Container>

            </Col>
          </Container>
        </Row>
      </Container>

      <br></br>

      {/* Set Supplier Limit */}
      <Container>
        <Row>
          <Col>
            <h4>Set Supplier Limit: </h4>
          </Col>
          <Container>
            <Col>
              <Container>
                <Row>
                  <Col>
                    <Form.Control onChange={(e) => setSupplierName(e.target.value)} size="md" required type="text" placeholder="Enter Supplier Name" />
                  </Col>
                  <Col>
                    <Form.Control onChange={(e) => setSupplierLimit(e.target.value)} size="md" required type="text" placeholder="Enter Supplier Limit" />
                  </Col>
                </Row>
              </Container>
              <br></br>
              <Container>
                <Row>
                  <Col>
                    <div>
                      <Button onClick={setSupplierLimits} variant="primary" size="md">
                        Set Supplier Limits!
                      </Button>
                    </div>
                  </Col>
                  <Col>
                    {supplierLimitAdded}
                  </Col>
                </Row>
              </Container>
            </Col>
          </Container>
        </Row>
      </Container>

      <br></br>

      {/* Get Supplier Limit */}
      <Container>
        <Row>
          <Col>
            <h4>Get Supplier Limit: </h4>
          </Col>
          <Container>
            <Col>
              <Container>
                <Row>
                  <Col>
                    <Form.Control onChange={(e) => setSupplierName(e.target.value)} size="md" required type="text" placeholder="Enter Supplier Name" />
                  </Col>
                  <Col>
                  </Col>
                </Row>
              </Container>
              <br></br>
              <Container>
                <Row>
                  <Col>
                    <div>
                      <Button onClick={getSupplierLimits} variant="primary" size="md">
                        Get Supplier Limits!
                      </Button>
                    </div>
                  </Col>
                  <Col>
                    <h4>{supplierLimitFetched}</h4>
                  </Col>
                </Row>
              </Container>
            </Col>
          </Container>
        </Row>
      </Container>

      <br></br>

      {/* Set Bid For Vedanta */}
      <Container>
        <Row>
          <Col>
            <h4>Set Bid for Vedanta: </h4>
          </Col>
          <Container>
            <Col>
              <Container>
                <Row>
                  <Col>
                    <Form.Control onChange={(e) => setBidderName(e.target.value)} size="md" required type="text" placeholder="Enter Bidder Name" />
                  </Col>
                  <Col>
                    <Form.Control onChange={(e) => setBidderPrice(e.target.value)} size="md" required type="text" placeholder="Enter Bidder Price" />
                  </Col>
                  <Col>
                    <Form.Control onChange={(e) => setBidderQuantity(e.target.value)} size="md" required type="text" placeholder="Enter Bidder Quantity" />
                  </Col>
                </Row>
              </Container>
              <br></br>
              <Container>
                <Row>
                  <Col>
                    <div>
                      <Button onClick={setBidsForVedanta} variant="primary" size="md">
                        Set Bid for Vedanta!
                      </Button>
                    </div>
                  </Col>
                  <Col>
                    {bidsForVedantaAdded}
                  </Col>
                </Row>
              </Container>
            </Col>
          </Container>
        </Row>
      </Container>

      <br></br>

      {/* Set Bid For MRF */}
      <Container>
        <Row>
          <Col>
            <h4>Set Bid for MRF: </h4>
          </Col>
          <Container>
            <Col>
              <Container>
                <Row>
                  <Col>
                    <Form.Control onChange={(e) => setBidderName(e.target.value)} size="md" required type="text" placeholder="Enter Bidder Name" />
                  </Col>
                  <Col>
                    <Form.Control onChange={(e) => setBidderPrice(e.target.value)} size="md" required type="text" placeholder="Enter Bidder Price" />
                  </Col>
                  <Col>
                    <Form.Control onChange={(e) => setBidderQuantity(e.target.value)} size="md" required type="text" placeholder="Enter Bidder Quantity" />
                  </Col>
                </Row>
              </Container>
              <br></br>
              <Container>
                <Row>
                  <Col>
                    <div>
                      <Button onClick={setBidsForMRF} variant="primary" size="md">
                        Set Bid for MRF!
                      </Button>
                    </div>
                  </Col>
                  <Col>
                    {bidsForMRFAdded}
                  </Col>
                </Row>
              </Container>
            </Col>
          </Container>
        </Row>
      </Container>

      <br></br>

      {/* Set Bid For Ceat */}
      <Container>
        <Row>
          <Col>
            <h4>Set Bid for Ceat: </h4>
          </Col>
          <Container>
            <Col>
              <Container>
                <Row>
                  <Col>
                    <Form.Control onChange={(e) => setBidderName(e.target.value)} size="md" required type="text" placeholder="Enter Bidder Name" />
                  </Col>
                  <Col>
                    <Form.Control onChange={(e) => setBidderPrice(e.target.value)} size="md" required type="text" placeholder="Enter Bidder Price" />
                  </Col>
                  <Col>
                    <Form.Control onChange={(e) => setBidderQuantity(e.target.value)} size="md" required type="text" placeholder="Enter Bidder Quantity" />
                  </Col>
                </Row>
              </Container>
              <br></br>
              <Container>
                <Row>
                  <Col>
                    <div>
                      <Button onClick={setBidsForCeat} variant="primary" size="md">
                        Set Bid for Ceat!
                      </Button>
                    </div>
                  </Col>
                  <Col>
                    {bidsForCeatAdded}
                  </Col>
                </Row>
              </Container>
            </Col>
          </Container>
        </Row>
      </Container>

      <br></br>

      {/* Get Vedanta Bid */}
      <Container>
        <Row>
          <Col>
            <h4>Get Vedanta Bid: </h4>
          </Col>
          <Container>
            <Col>
              <Container>
                <Row>
                  <Col>
                    <Form.Control onChange={(e) => setBidderName(e.target.value)} size="md" required type="text" placeholder="Enter Supplier Name" />
                  </Col>
                  <Col>
                  </Col>
                </Row>
              </Container>
              <br></br>
              <Container>
                <Row>
                  <Col>
                    <div>
                      <Button onClick={getBidsForVedanta} variant="primary" size="md">
                        Get Vedanta Bids!
                      </Button>
                    </div>
                  </Col>
                  <Col>
                    {bidsForVedantaFetched}
                  </Col>
                </Row>
              </Container>
            </Col>
          </Container>
        </Row>
      </Container>

      <br></br>

      {/* Get MRF Bid */}
      <Container>
        <Row>
          <Col>
            <h4>Get MRF Bid: </h4>
          </Col>
          <Container>
            <Col>
              <Container>
                <Row>
                  <Col>
                    <Form.Control onChange={(e) => setBidderName(e.target.value)} size="md" required type="text" placeholder="Enter Supplier Name" />
                  </Col>
                  <Col>
                  </Col>
                </Row>
              </Container>
              <br></br>
              <Container>
                <Row>
                  <Col>
                    <div>
                      <Button onClick={getBidsForMRF} variant="primary" size="md">
                        Get MRF Bids!
                      </Button>
                    </div>
                  </Col>
                  <Col>
                    {bidsForMRFFetched}
                  </Col>
                </Row>
              </Container>
            </Col>
          </Container>
        </Row>
      </Container>

      <br></br>

      {/* Get Ceat Bid */}
      <Container>
        <Row>
          <Col>
            <h4>Get Ceat Bid: </h4>
          </Col>
          <Container>
            <Col>
              <Container>
                <Row>
                  <Col>
                    <Form.Control onChange={(e) => setBidderName(e.target.value)} size="md" required type="text" placeholder="Enter Supplier Name" />
                  </Col>
                  <Col>
                  </Col>
                </Row>
              </Container>
              <br></br>
              <Container>
                <Row>
                  <Col>
                    <div>
                      <Button onClick={getBidsForCeat} variant="primary" size="md">
                        Get Ceat Bid!
                      </Button>
                    </div>
                  </Col>
                  <Col>
                    {bidsForCeatFetched}
                  </Col>
                </Row>
              </Container>
            </Col>
          </Container>
        </Row>
      </Container>

      <br></br>

      {/* Distribute Optimally */}
      <Container>
        <Row>
          <Col>
            <h4>Distribute Optimally: </h4>
          </Col>
          <Container>
            <Col>
              <Container>
                <Row>
                  <Col>
                    <div>
                      <Button onClick={distributeOptimally} variant="primary" size="md">
                        Distribute Optimally!
                      </Button>
                    </div>
                  </Col>
                  <Col>
                    {isDistributed}
                  </Col>
                </Row>
              </Container>
            </Col>
          </Container>
        </Row>
      </Container>

      <br></br>

      {/* Manufacture Cars */}
      <Container>
        <Row>
          <Col>
            <h4>Manufacture Cars: </h4>
          </Col>
          <Container>
            <Col>
              <Container>
                <Row>
                  <Col>
                    <div>
                      <Button onClick={manufactureCars} variant="primary" size="md">
                        Manufacture Cars!
                      </Button>
                    </div>
                  </Col>
                  <Col>
                    {isManufactured}
                  </Col>
                </Row>
              </Container>
            </Col>
          </Container>
        </Row>
      </Container>

      <br></br>

      {/* Get Manufaturer's Car Limit */}
      <Container>
        <Row>
          <Col>
            <h4>Get Manufaturer's Car Limit: </h4>
          </Col>
          <Container>
            <Col>
              <Container>
                <Row>
                  <Col>
                    <Form.Control onChange={(e) => setManufacturerName(e.target.value)} size="md" required type="text" placeholder="Enter Manufacturer Name" />
                  </Col>
                  <Col>
                  </Col>
                </Row>
              </Container>
              <br></br>
              <Container>
                <Row>
                  <Col>
                    <div>
                      <Button onClick={getManufacturedCarsLimit} variant="primary" size="md">
                        Get Manufacturer Limit!
                      </Button>
                    </div>
                  </Col>
                  <Col>
                    {carsLimit}
                  </Col>
                </Row>
              </Container>
            </Col>
          </Container>
        </Row>
      </Container>

      <br></br>

      {/* Buy Car */}
      <Container>
        <Row>
          <Col>
            <h4>Buy Car: </h4>
          </Col>
          <Container>
            <Col>
              <Container>
                <Row>
                  <Col>
                    <Form.Control onChange={(e) => setBuyerName(e.target.value)} size="md" required type="text" placeholder="Enter Buyer Name" />
                  </Col>
                  <Col>
                    <Form.Control onChange={(e) => setManufacturerName(e.target.value)} size="md" required type="text" placeholder="Enter Manufacturer Name" />
                  </Col>
                </Row>
              </Container>
              <br></br>
              <Container>
                <Row>
                  <Col>
                    <div>
                      <Button onClick={buyCar} variant="primary" size="md">
                        Buy Car!
                      </Button>
                    </div>
                  </Col>
                  <Col>
                    {/* {carIdBought} */}
                  </Col>
                </Row>
              </Container>
            </Col>
          </Container>
        </Row>
      </Container>

      <br></br>

      {/* Get Car Indexed bought by Customer */}
      <Container>
        <Row>
          <Col>
            <h4>Get Car Indexed bought by Customer: </h4>
          </Col>
          <Container>
            <Col>
              <Container>
                <Row>
                  <Col>
                    <Form.Control onChange={(e) => setCustomerName(e.target.value)} size="md" required type="text" placeholder="Enter Customer Name" />
                  </Col>
                  <Col>
                  </Col>
                </Row>
              </Container>
              <br></br>
              <Container>
                <Row>
                  <Col>
                    <div>
                      <Button onClick={getUserBoughtIndexes} variant="primary" size="md">
                        Get Car Indexed bought by Customer!
                      </Button>
                    </div>
                  </Col>
                  <Col>
                    {carIndexesBought}
                  </Col>
                </Row>
              </Container>
            </Col>
          </Container>
        </Row>
      </Container>

      <br></br>

      {/* Get Car Details */}
      <Container>
        <Row>
          <Col>
            <h4>Get Car Details: </h4>
          </Col>
          <Container>
            <Col>
              <Container>
                <Row>
                  <Col>
                    <Form.Control onChange={(e) => setCarId(e.target.value)} size="md" required type="text" placeholder="Enter Car Index" />
                  </Col>
                  <Col>
                  </Col>
                </Row>
              </Container>
              <br></br>
              <Container>
                <Row>
                  <Col>
                    <div>
                      <Button onClick={getCarDetails} variant="primary" size="md">
                        Get Car Details!
                      </Button>
                    </div>
                  </Col>
                  <Col>
                    {carDetails != null ? <div><pre>{JSON.stringify(carDetails, null, 2)}</pre></div> : <></>}
                  </Col>
                </Row>
              </Container>
            </Col>
          </Container>
        </Row>
      </Container>

      <br></br>
    </div>
  );
}

export default App;