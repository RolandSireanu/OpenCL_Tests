import BlockChain
import Block
from datetime import datetime

class Miner:    

    def __init__(self,blockChain):
        self.bc = blockChain;
        Miner.TARGET = BlockChain.BlockChain.TARGET;

    def mineNewBlock(self, data):
        now = datetime.now();
        currentTime = now.strftime("%H:%M:%S");
        lastBlock = self.bc.lastBlock;
        prevHash = lastBlock.computeHash();
        lastId = lastBlock.id;

        tempBlock = Block.Block(lastId+1, data, 0, currentTime, prevHash);
        while(tempBlock.computeHash()[:Miner.TARGET] != self.generateTarget()):              
            now = datetime.now();
            currentTime = now.strftime("%H:%M:%S");
            tempBlock.timestamp = currentTime;
            tempBlock.proof = tempBlock.proof + 1;            
        print(tempBlock)
        self.bc.addBlockToChain(tempBlock);


    def generateTarget(self):
        result="";

        
        for i in range(Miner.TARGET):
            result = result + "0";
        
        return result;