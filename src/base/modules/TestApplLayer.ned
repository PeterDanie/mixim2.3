package org.mixim.base.modules;


// Module which is responsible for Addressresolution
simple TestApplLayer like IBaseApplLayer
{
    parameters:
        @class(TestApplLayer);
        int headerLength @unit("bit"); // length of the application message header (in bits)
    gates:
        input lowerLayerIn; // from network layer
        output lowerLayerOut; // to network layer
        input lowerControlIn; // control from network layer
        output lowerControlOut; // control to network layer
}

