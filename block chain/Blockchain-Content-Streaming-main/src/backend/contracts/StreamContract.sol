// SPDX-License-Identifier: MIT
pragma solidity >=0.4.22 <0.9.0;

contract StreamContract {

  struct ContentInfo {
    string fileLocation;
    uint price;
    address payable owner;
    address[] usersWithAccess;
    // mapping(uint => address[]) usersWithAccess;
  }

  struct UserInfo {
    uint[] addedContentsId;
    uint[] boughtContentsId;
  }

  uint public contentId;
  address[] usersWithAccess;
  uint[] addedContentsId;
  uint[] boughtContentsId;

  mapping(uint => ContentInfo) public contents;
  mapping(address => UserInfo) users;

  constructor() {
    contentId = 0;
  }

  function addFile(string memory fileLocation, uint price) public returns (uint) {

    contentId++;

    contents[contentId] = ContentInfo(
      fileLocation,
      price,
      payable(msg.sender),
      usersWithAccess
    );
    contents[contentId].usersWithAccess.push(msg.sender);

    users[msg.sender] = UserInfo(
      addedContentsId,
      boughtContentsId
    );
    users[msg.sender].addedContentsId.push(contentId);

    return contentId;
  }

  function getUserWithAccess(uint id) public view returns (address[] memory) {
    return contents[id].usersWithAccess;
  }

  function getUsersPurchases() public view returns (uint[] memory) {
    return users[msg.sender].boughtContentsId;
  }

  function getUsersAddedContents() public view returns (uint[] memory) {
    return users[msg.sender].addedContentsId;
  }

  function getOwner(uint id) public view returns (address) {
    return contents[id].owner;
  }

  function giveAccess(uint id) public {
    contents[id].usersWithAccess.push(msg.sender);

    users[msg.sender].boughtContentsId.push(id);
  }

  function checkAccess(uint id, address userAddress) public view returns (bool) {
    for (uint i = 0; i < contents[id].usersWithAccess.length; i++) {
      if (contents[id].usersWithAccess[i] == userAddress) {
        return true;
      }
    }
    return false;
  }

  function payPrice(uint id) public payable returns(bool) {
    require(msg.value >= contents[id].price, "Not enough Ether");
    address payable owner = contents[id].owner;
    owner.transfer(contents[id].price);

    giveAccess(id);

    return true;
  }
}