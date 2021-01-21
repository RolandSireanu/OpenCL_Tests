import hashlib
import ProofFinding
import ipdb


class Block:

    def __init__(self, id, data, proof, timeStamp, prevHash):
        self.id = id;
        self.data = data;
        self.proof = proof;
        self.timestamp = timeStamp;
        self.prevHash = prevHash;
        self.proofFinding = ProofFinding.Proof();

    def __repr__(self):
        return "{}-{}-{}-{}-{}".format(self.id, self.data, self.proof, self.timestamp, self.prevHash);

    
    def computeHash(self):
        blockData = "{}{}{}{}{}".format(self.id, self.data, self.proof, self.timestamp, self.prevHash);
        bytesBlock = blockData.encode();
        
        
        # for i in range(100000) :
        #     h = self.proofFinding.computeSha256(str(i));
        #     print("{} , {}".format(len(h),i))

        # print("{} = {}".format(1689,hashlib.sha256("1654".encode()).hexdigest()));
        # print("{} = {}".format(1689,self.proofFinding.computeSha256("1654")));

        # ipdb.set_trace()

        return self.proofFinding.computeSha256(blockData);        
        
        #return hashlib.sha256(bytesBlock).hexdigest();