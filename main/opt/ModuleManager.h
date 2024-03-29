#ifndef OPT_MODMAN_H
#define OPT_MODMAN_H

#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"


// Include the passes
#include "opt/passes/RshCallSiteCallerCalleeInfo.h"
#include "opt/passes/RshCallSiteCounter.h"
#include "opt/passes/RshArgumentTracking.h"
#include "opt/passes/RshArgumentEffectSimple.h"
#include "opt/passes/FunctionCallBreathFirst.h"

#include "utils/RshBuiltinsMap.h"

using namespace llvm;


class ModuleManager {
  private:
    ModulePassManager MPM;
    LoopAnalysisManager LAM;
    FunctionAnalysisManager FAM;
    CGSCCAnalysisManager CGAM;
    ModuleAnalysisManager MAM;
    PassBuilder PB;
    llvm::Module & mod;
  public:
    
    ModuleManager(llvm::Module & m) : mod(m) {
      init();
    }

    void init() {
      PB.registerModuleAnalyses(MAM);
      PB.registerCGSCCAnalyses(CGAM);
      PB.registerFunctionAnalyses(FAM);
      PB.registerLoopAnalyses(LAM);
      PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

      // Add analysis passes
      MAM.registerPass([&] { return RshCallSiteCounter(); });
      // MAM.registerPass([&] { return RshCallSiteCallerCalleeInfo(); });
      // MAM.registerPass([&] { return RshArgumentTracking(); });
      MAM.registerPass([&] { return RshArgumentEffectSimple(); });
      MAM.registerPass([&] { return FunctionCallBreathFirst(); });
    }

    RshCallSiteCounter::Result getRshCallSiteCounterRes() {
      return MAM.getResult<RshCallSiteCounter>(mod);
    }

    RshArgumentEffectSimple::Result getRshArgumentEffectSimpleRes() {
      return MAM.getResult<RshArgumentEffectSimple>(mod);
    }

    FunctionCallBreathFirst::Result getFunctionCallBreathFirstRes() {
      return MAM.getResult<FunctionCallBreathFirst>(mod);
    }

    void runPasses() {
      MPM.run(mod, MAM);

      // MAM.getResult<FunctionCallBreathFirst>(mod);

      // auto argTrackRes = MAM.getResult<RshArgumentEffectSimple>(mod);
      // for (auto & ele : argTrackRes) {
      //   auto currFun = ele.first;
      //   auto currFunData = ele.second;
      //   std::cout << "        ArgData: " << currFun->getName().str() << std::endl;
      //   for (auto & data : currFunData) {
      //     unsigned argIdx = data.first;
      //     auto calledFuns = data.second; 
      //     std::cout << "          [" << data.first << "]: ";
      //     for (auto & funName : calledFuns) {
      //       std::cout << funName << " ";
      //     }
      //     std::cout << std::endl;
      //   }
      // }
      
      // auto argTrackRes = MAM.getResult<RshArgumentTracking>(m);
      // for (auto & ele : argTrackRes) {
      //   auto currFun = ele.first;
      //   auto currFunData = ele.second;
      //   std::cout << "        ArgData: " << currFun->getName().str() << std::endl;
      //   for (auto & data : currFunData) {
      //     std::cout << "          [" << data.first << "]: " << std::endl;
      //     int level = 0;
      //     for (auto & node : data.second) {
      //       std::cout << "             level: " << level++ << ": " << node.getNodeCompressedName() << std::endl;
      //     }
      //   }
      // }
      
      // auto result = MAM.getResult<RshCallSiteCallerCalleeInfo>(m);
      // for (auto & callSiteInfo : result) {
      //   // outs() << callSiteInfo << "\n";
      //   std::cout << "        call_site(" << callSiteInfo.first().str() << "): " << std::endl;
      //   std::cout << "          [";
      //   for (auto & caller : callSiteInfo.second.first) {
      //     std::cout << caller << " ";
      //   }
      //   std::cout << "]" << std::endl;

      //   std::cout << "          [";
      //   for (auto & callee : callSiteInfo.second.second) {
      //     std::cout << callee << " ";
      //   }
      //   std::cout << "]" << std::endl;
      // }

      // auto callSiteCountRes = MAM.getResult<RshCallSiteCounter>(m);

      // unsigned weight = 0;

      // for (auto & ele : callSiteCountRes) {
      //   weight += RshBuiltinWeights::getWeight(ele.first().str());
      //   // std::cout << "          " << ele.first().str() << "[" << RshBuiltinWeights::getWeight(ele.first().str()) << "]" << ":" << ele.second << "" << std::endl;
      // }
      // std::cout << "            WEIGHT: " << weight << std::endl;

    }


};

#endif