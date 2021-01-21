#/usr/bin/python3
import BlockChain
import Block
import Miner
from datetime import datetime


print("Hello world")
b = BlockChain.BlockChain()
m = Miner.Miner(b);
print(b.lastBlock);
m.mineNewBlock({"123fdssd3":"1dfds32fds","amount":0.00231});
m.mineNewBlock({"b23fdssd3":"1dfds32fds","amount":0.00231});
m.mineNewBlock({"a3fdssd3":"1dfds32fds","amount":0.00231});
m.mineNewBlock({"fdssd3":"1dfds32fds","amount":0.00431});
m.mineNewBlock({"c23fdssd3":"1dfds32fds","amount":0.01231});
m.mineNewBlock({"d23fdssd3":"1dfds32fds","amount":0.90231});

# now = datetime.now();
# t = now.strftime("%H:%M:%S");
# newBlock = Block.Block(1,{"from":"Roland", "to":"Alexandru"}, 0, t, b.lastBlock.computeHash());

# h = newBlock.computeHash();
# while(h[:5] != "00000"):
#     #print(h[:5]);  
#     newBlock.proof += 1;
#     h = newBlock.computeHash();
# #print(h);
# b.addBlockToChain(newBlock);

