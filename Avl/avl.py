import unittest
from collections import Counter
from dataclasses import dataclass
from enum import IntEnum
import random
from typing import Optional


class Direction(IntEnum):
    Left = -1
    Right = 1


@dataclass
class AvlNode:
    value: int
    balance: int
    _children: list[Optional['AvlNode']]

    @property
    def left(self):
        return self._children[0]

    @left.setter
    def left(self, node: 'AvlNode'):
        self._children[0] = node

    @property
    def right(self):
        return self._children[1]

    @right.setter
    def right(self, node: 'AvlNode'):
        self._children[1] = node

    def set_child(self, dir: Direction, node: 'AvlNode'):
        self._children[0 if dir == Direction.Left else 1] = node

    def get_child(self, dir: Direction):
        return self._children[0 if dir == Direction.Left else 1]

    def __getitem__(self, dir: Direction):
        return self.get_child(dir)

    def __setitem__(self, dir: Direction, node: 'AvlNode'):
        self.set_child(dir, node)


class AvlTree:
    def __init__(self):
        self.root = None

    def check_invariants(self):
        """
        Checks if invariants hold.
        Both ordering and balancing.
        Raises exception on broken invariants.
        """
        if self.root is not None:
            self._invariants_check(self.root)

    def _invariants_check(self, node: AvlNode) -> int:
        """
        Checks if invariants hold under given node.
        Both ordering and balancing.
        Raises exception on broken invariants.
        :return: Height of the node subtree (1 for node without children).
        """
        if node.left is not None:
            if node.left.value > node.value:
                raise Exception(f'Left value {node.left.value} is greater than value {node.value}.')
            left_height = self._invariants_check(node.left)
        else:
            left_height = 0

        if node.right is not None:
            if node.right.value < node.value:
                raise Exception(f'Right value {node.right.value} is less than value {node.value}.')
            right_height = self._invariants_check(node.right)
        else:
            right_height = 0

        if left_height == right_height:
            balance = 0
        elif left_height == right_height + 1:
            balance = -1
        elif left_height == right_height - 1:
            balance = 1
        else:
            raise Exception(f'Unbalanced node: left_height={left_height} right_height={right_height}')

        if balance != node.balance:
            raise Exception(f'Real balance is {balance} but node stores {node.balance}. left_height={left_height} right_height={right_height}')

        height = max(left_height, right_height) + 1
        return height

    def values(self) -> list[int]:
        """
        :return: Returns all values, sorted.
        """
        return self._values(self.root)

    def _values(self, node: AvlNode) -> list[int]:
        """
        :param node:    Node to return values for. Can be None.
        :return: Returns all values, under given node, sorted.
        """
        if node is None:
            return []
        return self._values(node.left) + [node.value] + self._values(node.right)

    def insert(self, value: int):
        """
        Inserts a value into the tree.
        """
        # Find the path to the place where we want to insert.
        current_node = self.root
        path_to_node: list[tuple[AvlNode, Direction]] = [] # Path from root to the node_to_insert, without the node_to_insert. (node, descend direction)
        while True:
            if current_node is None:
                node_to_insert = AvlNode(value, 0, [None, None])
                break
            going_dir = Direction.Left if value <= current_node.value else Direction.Right
            path_to_node.append((current_node, going_dir))
            current_node = current_node[going_dir]

        # Now we need to re-balance.
        rebalanced = False # If True we know the tree is fixed, but we still need to insert last node.
        while True:
            if len(path_to_node) == 0:
                self.root = node_to_insert
                break

            node, going_dir = path_to_node[-1]
            path_to_node.pop()
            node[going_dir] = node_to_insert

            if rebalanced:
                break

            node.balance += -1 if going_dir == Direction.Left else 1
            if node.balance == 0:
                break
            if node.balance in [1, -1]:
                node_to_insert = node
                continue

            node_to_insert, initial_balance_m = self.rebalance(node)
            if node_to_insert.balance == 0:
                rebalanced = True
                continue

    def remove(self, value: int) -> bool:
        """
        Removes a value from the tree.
        :return: True if deletion happened. False if value was not in the tree.
        """
        # Find the node we want to delete.
        current_node = self.root
        path_to_node: list[tuple[AvlNode, Direction]] = []  # Path from root to the node_to_remove, without the node_to_remove. (node, descend direction)
        while True:
            if current_node is None:
                return False

            if value == current_node.value:
                found_node = current_node
                break

            going_dir = Direction.Left if value < current_node.value else Direction.Right
            path_to_node.append((current_node, going_dir))
            current_node = current_node[going_dir]

        # Move children up to replace the removed value.
        # See figure in: https://en.wikipedia.org/wiki/Binary_search_tree#Deletion
        if (found_node.left is None) and (found_node.right is None): # a
            node_to_insert = None
        elif found_node.left is None: # b
            node_to_insert = found_node.right
        elif found_node.right is None: # c
            node_to_insert = found_node.left
        elif found_node.right.left is None: # d
            found_node.value = found_node.right.value
            found_node.right = found_node.right.right
            # found_node is the first node that needs balance fix.
            path_to_node.append((found_node, Direction.Right))
            node_to_insert = found_node.right
        else: # e
            # descend to minimal in right subtree
            path_to_node.append((found_node, Direction.Right))
            current_node = found_node.right
            while True:
                if current_node is None:
                    break
                path_to_node.append((current_node, Direction.Left))
                current_node = current_node.left

            minimal_node, _ = path_to_node[-1]
            minimal_node_parent, _ = path_to_node[-2]

            minimal_node_parent.left = minimal_node.right
            found_node.value = minimal_node.value

            path_to_node.pop() # minimal_node_parent is the first node that needs balance fix.
            node_to_insert = minimal_node_parent.left

        # Now we need to re-balance.
        rebalanced = False # If True we know the tree is fixed, but we still need to insert last node.
        while True:
            if len(path_to_node) == 0:
                self.root = node_to_insert
                break

            node, going_dir = path_to_node[-1]
            path_to_node.pop()
            node[going_dir] = node_to_insert

            if rebalanced:
                break

            node.balance += 1 if going_dir == Direction.Left else -1
            if node.balance in [1, -1]:
                break
            if node.balance == 0:
                node_to_insert = node
                continue

            node_to_insert, initial_balance_m = self.rebalance(node)
            if initial_balance_m == 0:
                rebalanced = True
                continue

        return True

    @staticmethod
    def rebalance(nodeT: AvlNode):
        """
        Rebalances a node.
        :param nodeT:   Node to rebalance. Must have balance -2 or 2.
        :return:        (node, initial_balance_m) - Node that should replace nodeT in the tree, initial balance of nodeM.

        4 cases:
            D = right when nodeT is +2, left otherwise.
            DM = right when nodeM is >0, left if <0, D if =0.

            Left:
                   nodeT (+2)                     nodeM
                  /        \                     /     \
                t1        nodeM (>=0)     nodeT        t3
                         /   \            /   \
                       t2    t3          t1   t2
            Right:
                   nodeT (-2)             nodeM
                  /        \             /     \
                nodeM (<=0) t1         t3      nodeT
               /   \                           /    \
            t3     t2                         t2    t1

            t1 = nodeT[-D]
            nodeM = nodeT[D]
            t2 = nodeM[-D]
            t3 = nodeM[D]

            nodeT.balance =  D if nodeM.balance == 0 else 0
            nodeM.balance = -D if nodeM.balance == 0 else 0

            nodeM[-D] = nodeT
            nodeM[D] = t3
            nodeT[-D] = t1
            nodeT[D] = t2

            Left+Right:
                   nodeT (+2)                        nodeB
                  /        \                       /       \
                t1        nodeM (<0)           nodeT         nodeM
                         /    \                /  \          /   \
                       nodeB  t4             t1   t2        t3   t4
                       /  \
                     t2   t3

            Right+Left:
                   nodeT (-2)                nodeB
                  /        \               /       \
                nodeM (>0)   t1        nodeM       nodeT
               /   \                   /  \        /   \
            t4    nodeB              t4   t3      t2   t1
                  /  \
                 t3   t2

            t1 = nodeT[-D]
            nodeM = nodeT[D]
            nodeB = nodeM[-D]
            t4 = nodeM[D]
            t2 = nodeB[-D]
            t3 = nodeB[D]

            if nodeB.balance == 0:
                nodeT.balance = 0
                nodeM.balance = 0
            else:
                nodeT.balance = -D if nodeB.balance == D else 0
                nodeM.balance =  D if nodeB.balance != D else 0
            nodeB.balance = 0

            nodeB[-D] = nodeT
            nodeB[D] = nodeM
            nodeT[-D] = t1
            nodeT[D] = t2
            nodeM[-D] = t3
            nodeM[D] = t4
        """
        if nodeT.balance == 2:
            D = Direction.Right
        elif nodeT.balance == -2:
            D = Direction.Left
        else:
            raise Exception(f'Balance was expected to be -2 or 2, but it: {nodeT.balance}')

        t1 = nodeT[-D]
        nodeM = nodeT[D]
        initial_balance_m = nodeM.balance

        DM = D
        if nodeM.balance > 0:
            DM = Direction.Right
        if nodeM.balance < 0:
            DM = Direction.Left

        if D == DM:
            # Rotate Left or Right
            t2 = nodeM[-D]
            t3 = nodeM[D]

            nodeM[-D] = nodeT
            nodeM[D] = t3
            nodeT[-D] = t1
            nodeT[D] = t2

            nodeT.balance =  D if nodeM.balance == 0 else 0
            nodeM.balance = -D if nodeM.balance == 0 else 0

            return nodeM, initial_balance_m

        # Rotate Left+Right or Right+Left
        nodeB = nodeM[-D]
        t4 = nodeM[D]
        t2 = nodeB[-D]
        t3 = nodeB[D]

        nodeB[-D] = nodeT
        nodeB[D] = nodeM
        nodeT[-D] = t1
        nodeT[D] = t2
        nodeM[-D] = t3
        nodeM[D] = t4

        if nodeB.balance == 0:
            nodeT.balance = 0
            nodeM.balance = 0
        else:
            nodeT.balance = -D if nodeB.balance == D else 0
            nodeM.balance =  D if nodeB.balance != D else 0
        nodeB.balance = 0

        return nodeB, initial_balance_m


class TestAvl(unittest.TestCase):
    def _action(self, avl: AvlTree, reference: Counter, value: int, insert: bool):
        if insert:
            avl.insert(value)
            avl.check_invariants()
            reference[value] += 1
        else:
            deleted = avl.remove(value)
            avl.check_invariants()
            self.assertEqual(deleted, reference[value] > 0)
            if deleted:
                reference[value] -= 1

        self.assertEqual(avl.values(), sorted(reference.elements()))

    def test_smoke(self):
        avl = AvlTree()
        reference = Counter()
        data = [1, 2, 10, 11, 2, 5, 0, 3, 3, 5]
        for d in data:
            self._action(avl, reference, d, True)

        avl.check_invariants()
        self._action(avl, reference, 1, False)
        self._action(avl, reference, 2, False)

        for d in data:
            self._action(avl, reference, d, False)

        for d in data:
            self._action(avl, reference, d, False)

    def test_random(self):
        avl = AvlTree()
        reference = Counter()
        for index in range(1000):
            value = random.randrange(0, 10)
            kind = random.choice([True, True, False])
            self._action(avl, reference, value, kind)

        final_values = avl.values()
        pass


def main():
    unittest.main()


if __name__ == "__main__":
    main()
