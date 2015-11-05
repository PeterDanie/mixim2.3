// ***************************************************************************
// 
// OppoNet Project
// This file is a part of the opponet project, jointly managed by the
// Laboratory for Communications Networks (LCN) at KTH in Stockholm, Sweden
// and Reykjavik University, Iceland.
//
// Copyright (C) 2008 Olafur Ragnar Helgason, Laboratory for Communication 
//                    Networks, KTH, Stockholm
//           (C)      Kristjan Valur Jonsson, Reykjavik University, Reykjavik
//
// ***************************************************************************
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License version 3
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not see <http://www.gnu.org/licenses/>.
//
// ***************************************************************************

//
// Trace mobility module.
//
// Used in conjunction with the NodeFactory object to implement the
// trace mobility model.
//
// Trace mobility uses externally generated traces or measurement 
// results to control a nodes movement. The module is initialized by the 
// factory at a certain location upon creation. It then supplies the module
// with its waypoint list and an optional destroy event. 
//
// @author  Olafur R. Helgason
// @author  Kristjan V. Jonsson
// @version 1.0 
//

// Peter package org.mixim.modules.mobility;

// Peter import org.mixim.base.modules.BaseMobility;

package inet.mobility.models; // Added Peter

import inet.mobility.models.MovingMobilityBase; // Added Peter

// Peter simple TraceMobility extends BaseMobility
simple TraceMobility extends MovingMobilityBase // Added Peter
{
  parameters:
      @class(TraceMobility);
      double initialX = default(-1); // Modified Peter
      double initialY  = default(-1); // Modified Peter
      double initialZ  = default(-1); // Modified Peter
      bool initFromDisplayString = default(false); // Added Peter      
      
      int nodeId = default(-1);	// Id of the node in the mobility tracefile
      bool interpolate = default(false);  // Interpolate between waypoints or not
      // Peter double updateInterval @unit("s") = default(0.1s);      
      updateInterval @unit("s") = default(0.1s); // Added Peter
}