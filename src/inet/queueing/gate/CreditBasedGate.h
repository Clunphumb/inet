//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#ifndef __INET_CREDITBASEDGATE_H
#define __INET_CREDITBASEDGATE_H

#include "inet/queueing/base/PacketGateBase.h"
#include "inet/queueing/gate/PeriodicGate.h"
#include "inet/common/ModuleRef.h"

namespace inet {
namespace queueing {

class INET_API CreditBasedGate : public PacketGateBase, public cListener
{
  public:
    static simsignal_t creditsChangedSignal;

  protected:
    ModuleRef<PeriodicGate> periodicGate;

    // parameters
    double idleCreditGainRate = NaN;
    double transmitCreditSpendRate = NaN;
    double transmitCreditLimit = NaN;
    double minCredit = NaN;
    double maxCredit = NaN;
    bool isAccumulatingBeforeGateClose;

    // state
    bool isTransmitting = false;
    bool isInterpacketGap = false;
    double currentCredit = NaN;
    double currentCreditGainRate = NaN;
    double lastCurrentCreditEmitted = NaN;
    simtime_t preClosingStartTime = -1;
    simtime_t lastCurrentCreditEmittedTime = -1;

    cMessage *changeTimer = nullptr;

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *message) override;
    virtual void finish() override;
    virtual void refreshDisplay() const override;

    virtual void processPacket(Packet *packet) override;
    virtual bool hasAvailablePacket() const { return provider->canPullSomePacket(inputGate->getPathStartGate()); }
    virtual bool isTransmissionGateOpen() const {return periodicGate->isOpen();}
    virtual bool canPacketBeTransmitted() const {return periodicGate->canPullSomePacket(outputGate->getPathEndGate());}
    virtual void updateCurrentCredit();
    virtual void updateCurrentCreditGainRate();
    virtual void isInPreClosingTime();
    virtual void emitCurrentCredit();
    virtual void scheduleChangeTimer();
    virtual void processChangeTimer();

  public:
    virtual ~CreditBasedGate() { cancelAndDelete(changeTimer); }

    virtual void handleCanPullPacketChanged(cGate *gate) override;

    virtual std::string resolveDirective(char directive) const override;

    virtual void receiveSignal(cComponent *source, simsignal_t signal, double value, cObject *details) override;
    virtual void receiveSignal(cComponent *source, simsignal_t signal, cObject *object, cObject *details) override;
    virtual void receiveSignal(cComponent *source, simsignal_t signal, bool value, cObject *details) override;
};

} // namespace queueing
} // namespace inet

#endif

