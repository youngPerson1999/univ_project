# myTeam.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


from captureAgents import CaptureAgent
import random, time, util
from game import Directions
import game

#################
# Team creation #
#################

def createTeam(firstIndex, secondIndex, isRed,
               first = 'DummyAgent', second = 'DummyAgent'):
  """
  This function should return a list of two agents that will form the
  team, initialized using firstIndex and secondIndex as their agent
  index numbers.  isRed is True if the red team is being created, and
  will be False if the blue team is being created.

  As a potentially helpful development aid, this function can take
  additional string-valued keyword arguments ("first" and "second" are
  such arguments in the case of this function), which will come from
  the --redOpts and --blueOpts command-line arguments to capture.py.
  For the nightly contest, however, your team will be created without
  any extra arguments, so you should make sure that the default
  behavior is what you want for the nightly contest.
  """

  # The following line is an example only; feel free to change it.
  return [eval(first)(firstIndex), eval(second)(secondIndex)]

##########
# Agents #
##########

class DummyAgent(CaptureAgent):
  """
  A Dummy agent to serve as an example of the necessary agent structure.
  You should look at baselineTeam.py for more details about how to
  create an agent as this is the bare minimum.
  """

  def registerInitialState(self, gameState):

    self.start = gameState.getAgentPosition(self.index)
    CaptureAgent.registerInitialState(self, gameState)
    
    self.boundary = (gameState.data.layout.width - 2) // 2

    if not self.red:  #red: 0~절반, Blue: 절반+1~끝
        self.boundary += 1

    self.boundaries = []
    for i in range(gameState.data.layout.height):
        if not gameState.hasWall(self.boundary, i):
            self.boundaries.append((self.boundary, i))

  def chooseAction(self, gameState):
      """
      Picks among the actions with the highest Q(s,a).
      """
      actions = gameState.getLegalActions(self.index)

      # You can profile your evaluation time by uncommenting these lines
      # start = time.time()
      values = [self.evaluate(gameState, a) for a in actions]
      # print 'eval time for agent %d: %.4f' % (self.index, time.time() - start)

      maxValue = max(values)
      bestActions = [a for a, v in zip(actions, values) if v == maxValue]

      return random.choice(bestActions)

  def getSuccessor(self, gameState, action):
    """
    Finds the next successor which is a grid position (location tuple).
    """
    successor = gameState.generateSuccessor(self.index, action)
    return successor

  def evaluate(self, gameState, action):
    # 공격이냐 수비냐에 따라 다르게 평가 초기 food = 20
    if self.index == 0: #more offensive
      food = 17
    else:   #more defensive
      food = 19
    # 상대가 많이 먹어서 food보다 낮아졌다면 수비적
    if len(self.getFoodYouAreDefending(gameState).asList()) < food:
      features = self.getDefensiveFeatures(gameState, action)
      weights = self.getDefensiveWeights()
    else: #아니라면 공격적
      features = self.getOffensiveFeatures(gameState, action)
      weights = self.getOffensiveWeights()
    return features*weights

  def getOffensiveFeatures(self, gameState, action):
    features = util.Counter()
    successor = self.getSuccessor(gameState, action)
    myState = successor.getAgentState(self.index)
    myPos = myState.getPosition()
    # 최대한 많이 움직이게 하기 위해서.
    if action == Directions.STOP: features['stop'] = 1
    # 2개이상 먹었다면 아군 진영으로 돌아와야함
    if gameState.getAgentState(self.index).numCarrying > 1:
      features['distanceToTeam'] = min([self.getMazeDistance(myPos, boundary) for boundary in self.boundaries])
    else:
      # 아니라면 음식을 우선적으로 먹게함.
      foodList = self.getFood(successor).asList()
      features['leftFood'] = len(foodList)
      features['distanceToFood'] = 9999
      for food in foodList:
        dist = self.getMazeDistance(myPos, food)
        if dist < features['distanceToFood']:
          features['distanceToFood'] = dist
    
    enemies = [successor.getAgentState(i) for i in self.getOpponents(successor)]
    ghosts = [enemy for enemy in enemies if not enemy.isPacman and enemy.getPosition() is not None and enemy.scaredTimer == 0]
    if len(ghosts) > 0:
      # 적이 있다면 회피하게 하기 위해 맵에서의 거리를 계산해서 가중치.
      dists = [self.getMazeDistance(myPos, ghost.getPosition()) for ghost in ghosts]
      features['distanceToGhost'] = 1 / min(dists)
    # 상대에게 당한자리라면 안가는것을 선호하도록함.
    if myPos == self.start:
      features['distanceToGhost'] = 9999

    features['successorScore'] = self.getScore(successor)

    return features

  def getDefensiveFeatures(self, gameState, action):
    features = util.Counter()
    successor = self.getSuccessor(gameState, action)
    myState = successor.getAgentState(self.index)
    myPos = myState.getPosition()

    # Computes distance to invaders we can see
    enemies = [successor.getAgentState(i) for i in self.getOpponents(successor)]
    invaders = [a for a in enemies if a.isPacman and a.getPosition() != None]
    features['numInvaders'] = len(invaders)
    print(myState.getCapsules)
    if myState.scaredTime > 0:
      print("scare")
    # features['closeGhost'] = -100
    # 적군이 아군진영으로 들어온 상황임.
    if len(invaders) > 0:
      dists = [self.getMazeDistance(myPos, a.getPosition()) for a in invaders]
      features['invaderDistance'] = min(dists)
      if myState.scaredTimer > 0:
            features['closeGhost'] = -100
    else:
      # 아니라면 최대한 경계쪽으로 가주는 것이 방어하기 쉬움
      closeX = self.boundary - myPos[0] if self.red else myPos[0] - self.boundary
      if closeX >= 0:
        features['closeBoundary'] = min([self.getMazeDistance(myPos, boundary) for boundary in self.boundaries])
      else:
        if closeX >= -2:
          features['closeBoundary'] = -1
    # 만약 상대 진영에 넘어가 있다면 돌아오게 해야함
    if myState.isPacman:
      enemies = [successor.getAgentState(i) for i in self.getOpponents(successor)]
      ghosts = [enemy for enemy in enemies if
                not enemy.isPacman and enemy.getPosition() is not None and enemy.scaredTimer == 0]
      if len(ghosts) > 0:
        dists = [self.getMazeDistance(myPos, ghost.getPosition()) for ghost in ghosts]
        if min(dists) <= 3:
          features['closeGhost'] = min(dists)


    return features
    
  def getOffensiveWeights(self):
    return {'successorScore': 10, 'distanceToFood': -1, 'distanceToGhost': -15, 'distanceToTeam': -1, 'leftFood': -20, 'stop':-100}
  def getDefensiveWeights(self):
    return {'numInvaders': -100, 'invaderDistance': -2, 'closeBoundary': -1, 'closeGhost': -150}
  