import pyopencl as cl 
import numpy as np
import ipdb


class Proof:

    TYPE_GPU = 0x04

    

    def __init__(self):
        platform = cl.get_platforms()[0];
        devices = platform.get_devices();
        self.dev = None;

        for d in devices:
            if(d.type == Proof.TYPE_GPU):
                print("GPU Found ! ");
                self.dev = d;

        self.context = cl.Context([self.dev])
        self.queue = cl.CommandQueue(self.context);

    def computeSha256(self, inputArg):
        metadata = np.array((
        64,
        1,
        len(inputArg))
        ).astype(np.uint32)

        inputData = np.array(list((ord(c) for c in inputArg)), dtype=np.byte)
        
        tempOutputData = np.empty(8).astype(np.uintc)
        outputData = np.empty(8).astype(np.uintc)

        #print(inputData);

        cl_metadata = cl.Buffer(self.context, cl.mem_flags.READ_ONLY | cl.mem_flags.COPY_HOST_PTR, hostbuf=metadata)
        cl_input = cl.Buffer(self.context, cl.mem_flags.READ_ONLY | cl.mem_flags.COPY_HOST_PTR, hostbuf=inputData)
        cl_output = cl.Buffer(self.context, cl.mem_flags.WRITE_ONLY, tempOutputData.nbytes);
        # Create three buffers (plans for areas of memory on the device)

        kernel = open("SHA256.cl").read();

        program = cl.Program(self.context, kernel).build()
        program.sha256_crypt_kernel(self.queue, (1,), None, cl_metadata, cl_input, cl_output);

        cl.enqueue_copy(self.queue, outputData, cl_output);
        self.queue.finish()
        #print([str(hex(nr)) for nr in outputData])
        hash = "";
        #print(outputData)
        for e in outputData:
            temp = str(hex(e))[2:];
            if(len(temp) < 8):
                for i in range(8-len(temp)):
                    temp = "0" + temp;
            hash += temp;
                        
        return hash;