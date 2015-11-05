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
//           (C) 2015 Peter Danielis, Laboratory for Communication 
//                    Networks, KTH, Stockholm 
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

#include "TraceMobility.h"
#include <FWMath.h>

Define_Module(TraceMobility);

void TraceMobility::initialize(int stage)
{
    MovingMobilityBase::initialize(stage);

    startPos = lastPosition;
    startTime = simTime();
    direction = Coord::ZERO;
    speed = 0;

    if (stage == 0) {
        nodeId = par("nodeId").longValue();
        if (nodeId == -1) {
            nodeId = this->getParentModule()->getId();
        }
        moving = false;
        interpolate = par("interpolate").boolValue();
        if (interpolate) {
            updateInterval = par("updateInterval");
        }
        updateEvent = new cMessage("updateEvent");
        // we want move-update events to have a higher priority than any other
        // events scheduled simultaneously.  This ensures that position is always
        // updated before other events.
        updateEvent->setSchedulingPriority(-1);
    } else if (stage == 1) {
        ev << "TraceMobility initialized at "<< startPos << endl;
    }
}

void TraceMobility::finish()
{
    MovingMobilityBase::finish();
    emitMobilityStateChangedSignal();
    cancelAndDelete(updateEvent);
}

void TraceMobility::handleSelfMessage(cMessage *msg)
{
   if (msg == updateEvent) {
        if(moving) {
            moveAndUpdate();
        } else {
            startMoving();
        }
    } else {
        MovingMobilityBase::handleSelfMessage(msg);
    }
}

void TraceMobility::move()
{
    if (moving)
    {
        step++;
        if (step == numSteps) {
            // Destination reached
            startPos = targetPos;
            startTime = simTime();
            speed = 0;
            direction = Coord(0,0);
            ev << "Target " << targetPos
                       << " reached, scheduling next waypoint" << endl;
            moving = false;

            scheduleNextWaypoint();
        } else if (step < numSteps) {
            // step forward
            startPos = stepTarget;
            startTime = simTime();

            stepTarget += stepSize;
            if (updateEvent->isScheduled()) {
                cancelEvent(updateEvent);
            }

            if (numSteps - step == 1) {
                scheduleAt(timeAtTarget, updateEvent);
            } else {
                scheduleAt(simTime() + updateInterval, updateEvent);
            }
        } else {
            throw cRuntimeError("step > numSteps in TraceMobility");
        }

        lastPosition = startPos;
    }
}

void TraceMobility::scheduleNextWaypoint()
{
    if (!eventList.empty()) {
        SetDestEv waypoint = eventList.front();

        if(updateEvent->isScheduled()) {
            throw cRuntimeError("Update event is already scheduled");
        }
        scheduleAt(waypoint.getTime(), updateEvent);
    }
}

void TraceMobility::startMoving()
{
    if (eventList.empty()) {
        throw cRuntimeError("No mobility events available!");
    }
    SetDestEv waypoint = eventList.front();
    eventList.pop_front();
    simtime_t now = simTime();
    if (waypoint.getTime() != now) {
        throw cRuntimeError("waypoint.getTime() != now");
    }
    if (waypoint.getTimeAtDest() < now) {
        throw cRuntimeError("timeAtDest (%s) is in the past (now = %s)",
                waypoint.getTimeAtDest().str().c_str(), now.str().c_str());
    }
    // TODO implement 3D support
    targetPos = Coord(waypoint.getX(), waypoint.getY());
    timeAtTarget = waypoint.getTimeAtDest();
    step = 0;
    if (targetPos == startPos) {
        numSteps = 1;
        speed = 0;
        moving = false;
        scheduleNextWaypoint();
    } else {
        Coord pos = startPos;
        speed = pos.distance(targetPos) / (timeAtTarget - now);

        direction = targetPos - startPos;
        moving = true;
        // calculate steps for interpolate or non-interpolate
        simtime_t nextEventTime;
        if (!interpolate) {
            numSteps = 1;
            nextEventTime = timeAtTarget;
        } else {
            // Get the number of steps needed to be covered.
            simtime_t travelTime = timeAtTarget - now;
            numSteps = static_cast < int >(ceil(travelTime.dbl() / updateInterval));
            if (numSteps > 1) {
                nextEventTime = now + updateInterval;
            } else {
                nextEventTime = timeAtTarget;
            }
        }
        if (updateEvent->isScheduled()) {
            cancelEvent(updateEvent);
        }

        scheduleAt(nextEventTime, updateEvent);
        ev << "Start moving from "<< startPos << " to " 
           << targetPos << " in " << numSteps << " steps "
           << " at speed=" << speed << endl;
    }
}

void TraceMobility::initializeTrace(const waypointEventsList* el)
{
    Enter_Method_Silent();
    if (el == NULL)
        return;
    // Copy the given event list to the internal list.
    eventList.resize(el->size());
    std::copy(el->begin(), el->end(), eventList.begin());
    scheduleNextWaypoint();
}
