const contracts = artifacts.require("StreamContract");

module.exports = function (deployer) {
  deployer.deploy(contracts);
};
