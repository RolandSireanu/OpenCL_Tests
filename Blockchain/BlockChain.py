
import Block
from datetime import datetime
import ipdb;


class BlockChain:

    TARGET=6

    def __init__(self):
        self.chain = [];
        self.buildFirstBlock(0, 0);

    def addBlockToChain(self, newBlock):
        
        if(self.__checkConstraints__(newBlock)):
            self.chain.append(newBlock);
            print(newBlock)

    
    def buildFirstBlock(self, prev_hash, proof):
        now = datetime.now();
        t = now.strftime("%H:%M:%S");
        b = Block.Block(0, {}, 0, t, 0);                
        self.chain.append(b);

    @staticmethod
    def __genTarget__(nrOfZeros):
        r = "";
        for i in range(nrOfZeros):                        
            r = r + "0";
        return r;


    def __checkConstraints__(self,block):
        lastBlock = self.chain[len(self.chain)-1];
        
        if(lastBlock.computeHash() != block.prevHash):
            return False;
        if(lastBlock.id >= block.id):
            return False;
        if(block.computeHash()[:BlockChain.TARGET] != BlockChain.__genTarget__(BlockChain.TARGET)):
            return False;
        return True;


    @property
    def lastBlock(self):
        return self.chain[len(self.chain)-1];
