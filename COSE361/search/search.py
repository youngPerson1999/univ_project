# search.py
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


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

import util

class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return  [s, s, w, s, w, w, s, w]

def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print("Start:", problem.getStartState())
    print("Is the start a goal?", problem.isGoalState(problem.getStartState()))
    print("Start's successors:", problem.getSuccessors(problem.getStartState()))
    """
    "*** YOUR CODE HERE ***"
    from util import Stack  #for using Stack
    s = Stack()
    visit = []                  #check if visited
    path = []                   #return path
    saveCurrPath = Stack()      #경로들을 저장합니다.
    curr = problem.getStartState()              #현재 위치는 스택위 top을 넣어줌
    while not problem.isGoalState(curr):            #현재 상태가 목적지가 아닐 때 계속 실행
        if curr not in visit:                       #이미 방문한 곳은 방문하지 않음
            visit.append(curr)                      #현재 위치 -> 방문한곳으로 넣어줌
            sucr = problem.getSuccessors(curr)      #searchAgent의 getSuccessors를 이용해서 다음에 갈 곳, 방향, 갈 곳의 cost들의 array를 sucr에 넣어준다
            for state, action, cost in sucr:        #suscr를 이용해 스택에 넣어주기 위해 반복문 사용
                s.push(state)                       #stack에 다음 상태(팩맨이 갈 곳)을 push
                saveCurrPath.push(path + [action])  #다음에 어떻게 할지를 path에 붙여서 path stack에 넣어줍니다
        curr = s.pop()                              #다음에 팩맨이 갈 곳 <- top of stack
        path = saveCurrPath.pop();                  #path = top of pathstack
    return (path)                                   #지금까지 들른 경로를 return
    #util.raiseNotDefined()

def breadthFirstSearch(problem):
    """Search the shallowest nodes in the search tree first."""
    "*** YOUR CODE HERE ***"
    from util import Queue
    q = Queue()                             #bfs는 큐를 이용해서 구현합니다.
    visit = []                              #check if visited
    path = []                               #return path
    saveCurrPath = Queue()                  #경로들의 모음을 저장합니다.
    curr = problem.getStartState()          #curr <- 팩맨의 처음 위치
    while not problem.isGoalState(curr):            #queue 빌때까지 실행해줍니다.
        if curr not in visit:                       #방문하지 않은 state만 실행해줍니다.
            visit.append(curr)                      #현재 위치 -> 방문한곳으로 넣어줌
            sucr = problem.getSuccessors(curr)      #searchAgent의 getSuccessors를 이용해서 다음에 갈 곳, 방향, 갈 곳의 cost들의 array를 sucr에 넣어준다
            for state, action, cost in sucr:        #suscr를 이용해 큐에 넣어주기 위해 반복문 사용합니다
                q.push(state)                       #queue에 다음 상태(팩맨이 갈 곳)을 push
                saveCurrPath.push(path + [action])  #다음에 어떻게 할지를 path에 붙여서 path queue에 넣어줍니다
        curr = q.pop()                              #다음에 팩맨이 갈 곳 <- first of queue
        path = saveCurrPath.pop()                   #path = first of path queue
    return (path)                                   #지금까지 들른 경로를 return
    #util.raiseNotDefined()

def uniformCostSearch(problem):
    """Search the node of least total cost first."""
    "*** YOUR CODE HERE ***"
    from util import PriorityQueue
    pq = PriorityQueue()                            #cost에 따라 작은 코스트부터 나와야하니 priorityQueue를 사용해서 ucs를 구현했습니다.
    visit = []                                      #check if visited
    path = []                                       #return path
    saveCurrPath = PriorityQueue()                  #경로들의 모음을 저장합니다
    pq.push(problem.getStartState(), 0)             #팩맨의 처음 위치, path cost(처음이니까 0)를 넣어줍니다
    curr = pq.pop()                                 #현재 상태는 위에서 넣은 것을 넣어줍니다.
    while not problem.isGoalState(curr):            #목적지에 도착할때까지 실행시켜줍니다.
        if curr not in visit:                       #방문하지 않은 state만 실행해줍니다.
            visit. append(curr)                     #현재 위치 -> 방문한곳으로 넣어줌
            sucr = problem.getSuccessors(curr)      #searchAgent의 getSuccessors를 이용해서 다음에 갈 곳, 방향, 갈 곳의 cost들의 array를 sucr에 넣어준다
            for state, action, cost in sucr:        #suscr를 이용해 우선순위큐에 넣어주기 위해 반복문 사용합니다
                nextCost = problem.getCostOfActions(path + [action])    #다음 path cost는 지금까지 와온 경로와 action을 취해 생기는 cost를 더해서 구해줍니다.
                if state not in visit:              #방문하지 않은 state만 넣어줍니다.
                    pq.push(state, nextCost)        #state와 path cost를 우선순위큐에 넣어줍니다.
                    saveCurrPath.push(path + [action], nextCost)    #path에 action과 nextCost를 push해줍니다.
        curr = pq.pop()                             #다음에 팩맨이 갈 곳 <- first of queue
        path = saveCurrPath.pop()                   #path는 지나온 경로와 가중치를 저장해줍니다.
    return (path)                                   #path를 return합니다.

def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0

def aStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""
    "*** YOUR CODE HERE ***"
    from util import PriorityQueue
    pq = PriorityQueue()                            #heuristic을 더하는 부분만 다를 뿐, 알고리즘 자체는 ucs와 상당히 유사하므로, path cost가 가장 작은 것을 선택한다는 점에서 우선순위큐를 사용해줍니다.
    visit = []                                      #check if visited
    path = []                                       #return path를 위해 빈 것
    saveCurrPath = PriorityQueue()                  #경로들의 모음을 저장합니다
    pq.push(problem.getStartState(), heuristic(problem.getStartState(), problem))             #처음 상태와 cost=heuristic(처음 팩맨의 위치)을 우선순위큐에 넣어줍니다
    curr = pq.pop()                                 #현재 상태에 위에서 넣은 값들을 넣어줍니다.
    while not problem.isGoalState(curr):            #목적지에 도착할때까지 실행시켜줍니다.
        if curr not in visit:                       #방문하지 않은 state만 실행해줍니다.
            visit.append(curr)                      #현재 위치 -> 방문한곳으로 넣어줌
            sucr = problem.getSuccessors(curr)      #searchAgent의 getSuccessors를 이용해서 다음에 갈 곳, 방향, 갈 곳의 cost들의 array를 sucr에 넣어준다
            for state, action, cost in sucr:        #searchAgent의 getSuccessors를 이용해서 다음에 갈 곳, 방향, 갈 곳의 cost들의 array를 sucr에 넣어준다    
                nextCost = problem.getCostOfActions(path + [action]) + heuristic(state, problem)    #nextCost는 (지금까지 경로 + 다음 위치)의 cost + 휴리스틱(다음 위치)이다
                if state not in visit:              #방문하지 않은 위치만 우선순위큐에 넣습니다.
                    pq.push(state, nextCost)        
                    saveCurrPath.push(path + [action], nextCost)
        curr = pq.pop()
        path = saveCurrPath.pop()
    return (path)
    #util.raiseNotDefined()


# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch
