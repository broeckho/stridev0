from pystride.stride.stride import NoGlobNoLocNoBeliefNoBehaviour
from pystride.stride.stride import NoGlobNoLocNoBeliefNoBehaviourBuilder

class SimulationUtils:
    def __init__(self):
        pass

    @staticmethod
    def getSimulator(runConfig, numThreads=1, trackIndexCase=False):
        """
        """
        gloInfoPolicy = runConfig.find('global_information_policy').text
        locInfoPolicy = runConfig.find('local_information_policy').text
        beliefPolicy = runConfig.find('belief_policy').text
        behaviourPolicy = runConfig.find('behaviour_policy').text

        if gloInfoPolicy == "NoGlobalInformation":
            if locInfoPolicy == "NoLocalInformation":
                if beliefPolicy == "NoBelief":
                    if behaviourPolicy == "NoBehaviour":
                        return NoGlobNoLocNoBeliefNoBehaviourBuilder.Build(runConfig, numThreads, trackIndexCase)
                    else:
                        print("Invalid configuration: no poloicy called " + behaviourPolicy)
                else:
                    print("Invalid configuration: no policy called " + beliefPolicy)
            else:
                print("Invalid configuration no policy called " + locInfoPolicy)
        else:
            print("Invalid configuration: no policy called " + gloInfoPolicy)
