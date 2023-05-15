import { useState } from 'react'
import { ethers } from "ethers"
import {
  BrowserRouter,
  Routes,
  Route
} from "react-router-dom";
import { Spinner } from 'react-bootstrap'

import './App.css';

import Navigation from './Navbar';
import AllContent from './AllContent.js';
import CreateContent from './CreateContent';
import MyContent from './MyContent';
import MyPurchases from './MyPurchases';

import StreamContractAbi from './backend/build/contracts/StreamContract.json'

function App() {
  const [loading, setLoading] = useState(true)
  const [account, setAccount] = useState(null)
  const [contract, setContract] = useState({})

  const contractAddress = "0x433557BE4418eAE028927Cd6886cBe6a06CA4Bac"

  // MetaMask Login/Connect
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
    console.log(StreamContractAbi);
    const contract = new ethers.Contract(contractAddress, StreamContractAbi.abi, signer)
    setContract(contract)
    setLoading(false)
  }

  return (
    <BrowserRouter>
      <div className="App">
        <>
          <Navigation web3Handler={web3Handler} account={account} />
        </>
        <div>
          {loading ? (
            <div style={{ display: 'flex', justifyContent: 'center', alignItems: 'center', minHeight: '80vh' }}>
              <Spinner animation="border" style={{ display: 'flex' }} />
              <p className='mx-3 my-0'>Awaiting Metamask Connection...</p>
            </div>
          ) : (
            <Routes>
              <Route path="/" element={
                <AllContent contract={contract} account={account} />
              } />
              <Route path="/create" element={
                <CreateContent contract={contract} />
              } />
              <Route path="/my-content" element={
                <MyContent contract={contract} account={account} />
              } />
              <Route path="/my-purchases" element={
                  <MyPurchases contract={contract} account={account} />
              } />
            </Routes>
          )}    
        </div>
      </div>
    </BrowserRouter>
  );
}

export default App;
