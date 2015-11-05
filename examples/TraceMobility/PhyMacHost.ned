package org.mixim.examples.TraceMobility;

import org.mixim.modules.node.WirelessNode;
import org.mixim.base.modules.IBaseApplLayer;
import inet.mobility.models.TraceMobility;

module PhyMacHost extends WirelessNode
{            
    parameters:
        nicType = "Nic80211";
        mobilityType = "TraceMobility";
        applicationType = "TestApplLayer"; // Implement the application here that moving hosts should execute     
        
    connections allowunconnected:
}

