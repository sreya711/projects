// SPDX-License-Identifier: MIT
pragma solidity >=0.4.22 <0.9.0;

contract SupplyChain {

  struct Wheel {
      uint id;
      string owner;
      string supplierName;
      uint price;
  }
  
  struct CarBody {
      uint id;
      string owner;
      string supplierName;
      uint price;
  }
  
  struct Car {
    uint id;
    string manufacturerName;
    Wheel wheel;
    CarBody carBody;
    uint price;
    bool isOwned;
  }

  struct Bid {
    uint price;
    uint quantity;
  }
  
  bool private isDistributed;
  bool private isManufactured;
  address public owner;
  uint private carId;
  uint private wheelId;
  uint private carBodyId;
  uint private carIndexAvailableForSaleWithTata;
  uint private carIndexAvailableForSaleWithMaruti;

  mapping(string => address) public clients;
  mapping(string => uint) public supplierLimit;
  mapping(string => Bid) private bidsForMRF;
  mapping(string => Bid) private bidsForVedanta;
  mapping(string => Bid) private bidsForCeat;
  mapping(string => bool) private bidStatusForMrf;
  mapping(string => bool) private bidStatusForCeat;
  mapping(string => bool) private bidStatusForVedanta;
  mapping(string => uint) private distributionQuantityByVedanta;
  mapping(string => uint) private distributionQuantityByMrf;
  mapping(string => uint) private distributionQuantityByCeat;
  mapping(uint => address) private customerData;
  mapping(string => uint[]) private indexOwnedByCustomer;

  Wheel[] public wheelWithTata;
  Wheel[] public wheelWithMaruti;
  CarBody[] public carBodyWithTata; 
  CarBody[] public carBodyWithMaruti;
  Car[] public carsWithTata; 
  Car[] public carsWithMaruti;

  modifier checkBidCompletion() {
    require(
     bidStatusForMrf["tata"] && 
     bidStatusForVedanta["tata"] &&
     bidStatusForVedanta["maruti"] &&
     bidStatusForCeat["maruti"],
      "The bidding is still in progess, Please wait !!"
    );
    _;
  }

  modifier checkDistribution() {
    require(
      isDistributed == true,
      "Wait for Distribution of Materials!!"
    );
    _;
  }

  modifier checkManufacturing() {
    require(
      isManufactured == true,
      "Wait for Manufacturing of Cars!!"
    );
    _;
  }

  modifier onlyOwner() {
    require(
      msg.sender == owner,
      "This function is restricted to the contract's owner"
    );
    _;
  }
  
  constructor() {
    owner = msg.sender;
    isDistributed = false;
    isManufactured = false;
    carIndexAvailableForSaleWithTata = 0;
    carIndexAvailableForSaleWithMaruti = 0;
    carId = 1;
    wheelId = 1;
    carBodyId = 1;
    bidStatusForMrf["tata"] = false;
    bidStatusForVedanta["tata"] = false;
    bidStatusForVedanta["maruti"] = false;
    bidStatusForCeat["maruti"] = false;
  }

  function min(uint a, uint b) internal pure returns (uint) {
    if (a <= b)
      return a;
    return b;
  }

  function setClient(string memory clientName, address clientAddress) onlyOwner public {
    clients[clientName] = clientAddress;
  }

  function getClient(string memory clientName) public view returns (address) {
    return clients[clientName];
  }
  
  function setSupplierLimit(string memory supplierName, uint supplyLimit) public {
    require(msg.sender == clients[supplierName], "Only the particular supplier can set their limit");
    supplierLimit[supplierName] = supplyLimit;
  }

  function getSupplierLimit(string memory supplierName) public view returns (uint) {
    return supplierLimit[supplierName];
  }

  function getCeatBid(string memory supplierName) checkBidCompletion public view returns (Bid memory) {
    return bidsForCeat[supplierName];
  }

  function getMRFBid(string memory supplierName) checkBidCompletion public view returns (Bid memory ) {
    return bidsForMRF[supplierName];
  }

  function getVedantaBid(string memory supplierName) checkBidCompletion public view returns (Bid memory) {
    return bidsForVedanta[supplierName];
  }
  
  function setBidForVedanta(string memory bidderName, uint price, uint quantity) public payable {
    require(msg.sender == clients[bidderName], "Only the particular bidder can set their bid");
    require(price >= 0 && quantity >= 0, "Set Proper Values !");
    bidsForVedanta[bidderName] = Bid (price, quantity);
    bidStatusForVedanta[bidderName] = true;
  } 

  function setBidForMRF(string memory bidderName, uint price, uint quantity) public payable {
    require(msg.sender == clients[bidderName], "Only the particular bidder can set their bid");
    require(price >= 0 && quantity >= 0, "Set Proper Values !");
    bidsForMRF[bidderName] = Bid (price, quantity);
    bidStatusForMrf[bidderName] = true;
  }

  function setBidForCeat(string memory bidderName, uint price, uint quantity) public payable {
    require(msg.sender == clients[bidderName], "Only the particular bidder can set their bid");
    require(price >= 0 && quantity >= 0, "Set Proper Values !");
    bidsForCeat[bidderName] = Bid (price, quantity);
    bidStatusForCeat[bidderName] = true;
  }

  function distributeOptimally() public  onlyOwner checkBidCompletion {

    uint wheelsForTata = min(supplierLimit["mrf"],bidsForMRF["tata"].quantity);
    uint wheelsForMaruti = min(supplierLimit["ceat"],bidsForCeat["maruti"].quantity);
    distributionQuantityByMrf["tata"] = wheelsForTata;
    distributionQuantityByCeat["maruti"] = wheelsForMaruti;

    if(wheelsForTata + wheelsForMaruti <= supplierLimit["vedanta"]) 
    {
      distributionQuantityByVedanta["tata"] = wheelsForTata;
      distributionQuantityByVedanta["maruti"] = wheelsForMaruti;
    }
    else
    {
      for(uint i = 0 ;i <supplierLimit["vedanta"] ; i++)
      {
        if(wheelsForTata > 0)
        {
          distributionQuantityByVedanta["tata"] += 1;
          wheelsForTata -= 1;
        }
        else
        {
          distributionQuantityByVedanta["maruti"] += 1;
          wheelsForMaruti -= 1;
        }
      }
    }

    for (uint i = 0 ; i < distributionQuantityByVedanta["tata"] ; i++) {
      carBodyWithTata.push(CarBody(carBodyId,"tata","vedanta",bidsForVedanta["tata"].price));
      carBodyId += 1;
    }
    for (uint i = 0 ; i < distributionQuantityByVedanta["maruti"] ; i++) {
      carBodyWithMaruti.push(CarBody(carBodyId,"maruti","vedanta",bidsForVedanta["maruti"].price));
      carBodyId += 1;
    }

    for (uint i = 0 ; i < distributionQuantityByMrf["tata"] ; i++) {
      wheelWithTata.push(Wheel(wheelId,"tata","mrf",bidsForMRF["tata"].price));
      wheelId += 1;
    }
    for (uint i = 0 ; i < distributionQuantityByCeat["maruti"] ; i++) {
      wheelWithMaruti.push(Wheel(wheelId,"maruti","ceat",bidsForCeat["maruti"].price));
      wheelId += 1;
    }

    isDistributed = true;
  }

  function manufactureCars() public  onlyOwner checkBidCompletion checkDistribution {
    for (uint i = 0 ; i < min(wheelWithMaruti.length,carBodyWithMaruti.length) ; i++) {
      carsWithMaruti.push(Car(carId,"maruti",wheelWithMaruti[i],carBodyWithMaruti[i],wheelWithMaruti[i].price+carBodyWithMaruti[i].price,false));
      carId += 1;
    }

    // carIndexAvailableForSaleWithTata = carId;
    for (uint i = 0 ; i < min(wheelWithTata.length,carBodyWithTata.length) ; i++) {
      carsWithTata.push(Car(carId,"tata",wheelWithTata[i],carBodyWithTata[i],wheelWithTata[i].price+carBodyWithTata[i].price,false));
      carId += 1;
    }

    isManufactured = true;
  }

  function getManufacturedCarsLimit(string memory manufacturerName) public checkBidCompletion checkDistribution checkManufacturing view returns (uint) {
    if(keccak256(abi.encodePacked("maruti")) == keccak256(abi.encodePacked(manufacturerName)))
    {
      return carsWithMaruti.length;
    }
    if(keccak256(abi.encodePacked("tata")) == keccak256(abi.encodePacked(manufacturerName)))
    {
      return carsWithTata.length;
    }
    return 0;
  }

  function purchaseCar(string memory buyerName, string memory manufacturerName) public checkBidCompletion checkDistribution checkManufacturing returns (uint) {
    uint carIdBought;
    require(msg.sender == clients[buyerName], "Only the particular buyer can buy from their wallets");
    if(keccak256(abi.encodePacked("maruti")) == keccak256(abi.encodePacked(manufacturerName)))
    {
      require(carIndexAvailableForSaleWithMaruti < carsWithMaruti.length,"No cars available with Maruti !!");
      require(carsWithMaruti[carIndexAvailableForSaleWithMaruti].isOwned == false, "Car already sold");

      carsWithMaruti[carIndexAvailableForSaleWithMaruti].isOwned = true;
      customerData[carsWithMaruti[carIndexAvailableForSaleWithMaruti].id] = msg.sender;
      carIdBought = carsWithMaruti[carIndexAvailableForSaleWithMaruti].id;
      carIndexAvailableForSaleWithMaruti += 1;

      indexOwnedByCustomer[buyerName].push(carIdBought);
    }

    if(keccak256(abi.encodePacked("tata")) == keccak256(abi.encodePacked(manufacturerName)))
    {
      require(carIndexAvailableForSaleWithTata < carsWithTata.length,"No cars available with Tata!!");
      require(carsWithTata[carIndexAvailableForSaleWithTata].isOwned == false, "Car already sold");

      carsWithTata[carIndexAvailableForSaleWithTata].isOwned = true;
      customerData[carsWithTata[carIndexAvailableForSaleWithTata].id] = msg.sender;
      carIdBought = carsWithTata[carIndexAvailableForSaleWithTata].id;
      carIndexAvailableForSaleWithTata += 1;

      indexOwnedByCustomer[buyerName].push(carIdBought);
    }

    return carIdBought;
  }


  function checkCarIndexOwnedByCustomer(string memory customerName) public checkBidCompletion checkDistribution checkManufacturing view returns (uint[] memory) {
    return indexOwnedByCustomer[customerName];
  }

  function checkCarDetails(uint carIndex) public checkBidCompletion checkDistribution checkManufacturing view returns (Car memory) {
    Car memory car;
    for (uint i = 0 ; i < carsWithMaruti.length ; i++) {
      if (carsWithMaruti[i].id == carIndex) {
        car = carsWithMaruti[i];
      }
    }

    for (uint i = 0 ; i < carsWithTata.length ; i++) {
      if (carsWithTata[i].id == carIndex) {
        car =  carsWithTata[i];
      }
    }

    return car;
  }
}
