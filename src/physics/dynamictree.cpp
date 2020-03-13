//
// Created by Washington on 13/03/2020.
//

#include "dynamictree.hpp"

DynamicTree::DynamicTree() {
    mNodes = (DynamicTreeNode*) malloc(sizeof(DynamicTree)*mNodeCapacity);
    memset(mNodes, 0, sizeof(DynamicTree)*mNodeCapacity);
    for(uint32_t i = 0; i < mNodeCapacity - 1; ++i) {
        mNodes[i].next = i + 1;
        mNodes[i].height = -1;
    }
    mNodes[mNodeCapacity - 1].next = -1;
    mNodes[mNodeCapacity - 1].height = -1;
}

DynamicTree::~DynamicTree() {
    free(mNodes);
}

int32_t DynamicTree::CreateProxy(const Bounds &bounds) {
    int32_t proxyId = AllocateNode();
    glm::vec3 r(0.1f);
    mNodes[proxyId].bounds.lowerBound = bounds.lowerBound - r;
    mNodes[proxyId].bounds.upperBound = bounds.upperBound + r;
    mNodes[proxyId].height = 1;
    mNodes[proxyId].moved = true;
    InsertLeaf(proxyId);
    return proxyId;
}

void DynamicTree::DestroyProxy(int32_t proxyId) {
    assert(0 <= proxyId && proxyId < mNodeCapacity);
    assert(mNodes[proxyId].IsLeaf());

    RemoveLeaf(proxyId);
    FreeNode(proxyId);
}

bool DynamicTree::MoveProxy(int32_t proxyId, const Bounds &aabb, const glm::vec3 &displacement) {
    assert(0 <= proxyId && proxyId < mNodeCapacity);

    assert(mNodes[proxyId].IsLeaf());

    // Extend AABB
    Bounds fatAABB;
    glm::vec3 r(0.1f);
    fatAABB.lowerBound = aabb.lowerBound - r;
    fatAABB.upperBound = aabb.upperBound + r;

    // Predict AABB movement
    glm::vec3 d = 2.0f * displacement;

    if (d.x < 0.0f) {
        fatAABB.lowerBound.x += d.x;
    } else {
        fatAABB.upperBound.x += d.x;
    }

    if (d.y < 0.0f) {
        fatAABB.lowerBound.y += d.y;
    } else {
        fatAABB.upperBound.y += d.y;
    }

    const Bounds& treeAABB = mNodes[proxyId].bounds;
    if (Physics::Contains(treeAABB, aabb)) {
        // The tree AABB still contains the object, but it might be too large.
        // Perhaps the object was moving fast but has since gone to sleep.
        // The huge AABB is larger than the new fat AABB.
        Bounds hugeAABB;
        hugeAABB.lowerBound = fatAABB.lowerBound - 4.0f * r;
        hugeAABB.upperBound = fatAABB.upperBound + 4.0f * r;

        if (Physics::Contains(hugeAABB, treeAABB)) {
            // The tree AABB contains the object AABB and the tree AABB is
            // not too large. No tree update needed.
            return false;
        }

        // Otherwise the tree AABB is huge and needs to be shrunk
    }

    RemoveLeaf(proxyId);

    mNodes[proxyId].bounds = fatAABB;

    InsertLeaf(proxyId);

    mNodes[proxyId].moved = true;

    return true;
}

bool DynamicTree::WasMoved(int32_t proxyId) const {
    assert(0 <= proxyId && proxyId < mNodeCapacity);
    return mNodes[proxyId].moved;
}

void DynamicTree::ClearMoved(int32_t proxyId) {
    assert(0 <= proxyId && proxyId < mNodeCapacity);
    mNodes[proxyId].moved = false;
}

const Bounds &DynamicTree::GetFatBounds(int32_t proxyId) const {
   assert(0 <= proxyId && proxyId < mNodeCapacity);
    return mNodes[proxyId].bounds;
}

void DynamicTree::Query(const std::function<bool(int32_t)> &callback, const Bounds &bounds) const {
    std::stack<int32_t> stack;
    stack.push(mRoot);

    while(!stack.empty()) {
        int32_t nodeId = stack.top();
        stack.pop();
        if(nodeId == -1) {
            continue;
        }

        const DynamicTreeNode *node = mNodes + nodeId;
        if(Physics::Intersects(node->bounds, bounds)) {
            if(node->IsLeaf()) {
                bool proceed = callback(nodeId);
                if(!proceed) {
                    return;
                }
            } else {
                stack.push(node->child1);
                stack.push(node->child2);
            }
        }
    }
}

void DynamicTree::RayCast(const std::function<float(const Ray&, uint32_t)> &callback, const Ray &ray) const {
    glm::vec3 p1 = ray.point1;
    glm::vec3 p2 = ray.point2;
    glm::vec3 r = glm::normalize(p2 - p1);
    float maxFraction = ray.maxFraction;

    std::stack<int32_t> stack;
    stack.push(mRoot);

    Ray rayTmp;
    while(!stack.empty()) {
        int32_t nodeId = stack.top();
        stack.pop();

        if(nodeId == -1) continue;

        const DynamicTreeNode *node = mNodes + nodeId;
        rayTmp.point1 = ray.point1;
        rayTmp.point2 = ray.point2;
        rayTmp.maxFraction = maxFraction;
        if(!Physics::IntersectRay(node->bounds, rayTmp)) continue;

        if(node->IsLeaf()) {
            float value = callback(rayTmp, nodeId);
            if(value < std::numeric_limits<float>::epsilon()) {
                return;
            }

            if(value > 0.0f && value < maxFraction) {
                maxFraction = value;
            }
        } else {
            stack.push(node->child1);
            stack.push(node->child2);
        }
    }
}

int32_t DynamicTree::GetHeight() const {
    if (mRoot == -1) {
        return 0;
    }

    return mNodes[mRoot].height;
}

int32_t DynamicTree::GetMaxBalance() const {
    int32_t maxBalance = 0;
    for (uint32_t i = 0; i < mNodeCapacity; ++i) {
        const DynamicTreeNode* node = mNodes + i;
        if (node->height <= 1) {
            continue;
        }

        assert(node->IsLeaf() == false);

        int32_t child1 = node->child1;
        int32_t child2 = node->child2;
        int32_t balance = glm::abs(mNodes[child2].height - mNodes[child1].height);
        maxBalance = (glm::max)(maxBalance, balance);
    }

    return maxBalance;
}

float DynamicTree::GetAreaRatio() const {
    if (mRoot == -1) {
        return 0.0f;
    }

    const DynamicTreeNode* root = mNodes + mRoot;
    float rootArea = root->bounds.GetPerimeter();

    float totalArea = 0.0f;
    for (uint32_t i = 0; i < mNodeCapacity; ++i) {
        const DynamicTreeNode* node = mNodes + i;
        if (node->height < 0) {
            // Free node in pool
            continue;
        }

        totalArea += node->bounds.GetPerimeter();
    }

    return totalArea / rootArea;
}

void DynamicTree::RebuildBottomUp() {
    auto nodes = (int32_t*)malloc(mNodeCount * sizeof(int32_t));
    int32_t count = 0;

    // Build array of leaves. Free the rest.
    for (uint32_t i = 0; i < mNodeCapacity; ++i) {
        if (mNodes[i].height < 0) {
            // free node in pool
            continue;
        }

        if (mNodes[i].IsLeaf()) {
            mNodes[i].parent = -1;
            nodes[count] = i;
            ++count;
        }
        else {
            FreeNode(i);
        }
    }

    while (count > 1) {
        float minCost = std::numeric_limits<float>::max();
        int32_t iMin = -1, jMin = -1;
        for (uint32_t i = 0; i < count; ++i) {
            Bounds aabbi = mNodes[nodes[i]].bounds;

            for (int32_t j = i + 1; j < count; ++j) {
                Bounds aabbj = mNodes[nodes[j]].bounds;
                Bounds b = aabbi + aabbj;
                float cost = b.GetPerimeter();
                if (cost < minCost) {
                    iMin = i;
                    jMin = j;
                    minCost = cost;
                }
            }
        }

        int32_t index1 = nodes[iMin];
        int32_t index2 = nodes[jMin];
        DynamicTreeNode* child1 = mNodes + index1;
        DynamicTreeNode* child2 = mNodes + index2;

        int32_t parentIndex = AllocateNode();
        DynamicTreeNode* parent = mNodes + parentIndex;
        parent->child1 = index1;
        parent->child2 = index2;
        parent->height = 1 + (glm::max)(child1->height, child2->height);
        parent->bounds = child1->bounds + child2->bounds;
        parent->parent = -1;

        child1->parent = parentIndex;
        child2->parent = parentIndex;

        nodes[jMin] = nodes[count-1];
        nodes[iMin] = parentIndex;
        --count;
    }

    mRoot = nodes[0];
    free(nodes);
}

void DynamicTree::ShiftOrigin(const glm::vec3 &newOrigin) {
    for (uint32_t i = 0; i < mNodeCapacity; ++i) {
        mNodes[i].bounds.lowerBound -= newOrigin;
        mNodes[i].bounds.upperBound -= newOrigin;
    }
}

int32_t DynamicTree::AllocateNode() {
    if(mFreeList == -1) {
        assert(mNodeCount == mNodeCapacity);

        auto oldNodes = mNodes;
        mNodeCapacity*=2;
        mNodes = (DynamicTreeNode*) malloc(sizeof(DynamicTree)*mNodeCapacity);
        memcpy(mNodes, oldNodes, sizeof(DynamicTreeNode)*mNodeCount);
        free(oldNodes);

        for (uint32_t i = mNodeCount; i < mNodeCapacity - 1; ++i)
        {
            mNodes[i].next = i + 1;
            mNodes[i].height = -1;
        }
        mNodes[mNodeCapacity-1].next = -1;
        mNodes[mNodeCapacity-1].height = -1;
        mFreeList = mNodeCount;
    }

    int32_t nodeId = mFreeList;
    mFreeList = mNodes[nodeId].next;
    mNodes[nodeId].parent = -1;
    mNodes[nodeId].child1 = -1;
    mNodes[nodeId].child2 = -1;
    mNodes[nodeId].height = 0;
    mNodes[nodeId].moved = false;
    mNodeCount++;
    return nodeId;
}

void DynamicTree::FreeNode(int32_t nodeId) {
    assert(0 <= nodeId && nodeId < mNodeCapacity);
    assert(mNodeCount > 0);
    mNodes[nodeId].next = mFreeList;
    mNodes[nodeId].height = -1;
    mFreeList = nodeId;
    --mNodeCount;
}

void DynamicTree::InsertLeaf(int32_t leaf) {
    ++mInsertionCount;

    if (mRoot == -1) {
        mRoot = leaf;
        mNodes[mRoot].parent = -1;
        return;
    }

    // Find the best sibling for this node
    Bounds leafAABB = mNodes[leaf].bounds;
    int32_t index = mRoot;
    while (!mNodes[index].IsLeaf()) {
        int32_t child1 = mNodes[index].child1;
        int32_t child2 = mNodes[index].child2;

        float area = mNodes[index].bounds.GetPerimeter();

        Bounds combinedAABB = mNodes[index].bounds + leafAABB;
        float combinedArea = combinedAABB.GetPerimeter();

        // Cost of creating a new parent for this node and the new leaf
        float cost = 2.0f * combinedArea;

        // Minimum cost of pushing the leaf further down the tree
        float inheritanceCost = 2.0f * (combinedArea - area);

        // Cost of descending into child1
        float cost1;
        if (mNodes[child1].IsLeaf()) {
            Bounds aabb = leafAABB + mNodes[child1].bounds;
            cost1 = aabb.GetPerimeter() + inheritanceCost;
        } else {
            Bounds aabb = leafAABB + mNodes[child1].bounds;
            float oldArea = mNodes[child1].bounds.GetPerimeter();
            float newArea = aabb.GetPerimeter();
            cost1 = (newArea - oldArea) + inheritanceCost;
        }

        // Cost of descending into child2
        float cost2;
        if (mNodes[child2].IsLeaf()) {
            Bounds aabb = leafAABB + mNodes[child2].bounds;
            cost2 = aabb.GetPerimeter() + inheritanceCost;
        } else  {
            Bounds aabb = leafAABB + mNodes[child2].bounds;
            float oldArea = mNodes[child2].bounds.GetPerimeter();
            float newArea = aabb.GetPerimeter();
            cost2 = newArea - oldArea + inheritanceCost;
        }

        // Descend according to the minimum cost.
        if (cost < cost1 && cost < cost2) {
            break;
        }

        // Descend
        if (cost1 < cost2) {
            index = child1;
        } else {
            index = child2;
        }
    }

    int32_t sibling = index;

    // Create a new parent.
    int32_t oldParent = mNodes[sibling].parent;
    int32_t newParent = AllocateNode();
    mNodes[newParent].parent = oldParent;
    mNodes[newParent].bounds = leafAABB + mNodes[sibling].bounds;
    mNodes[newParent].height = mNodes[sibling].height + 1;

    if (oldParent != -1) {
        // The sibling was not the root.
        if (mNodes[oldParent].child1 == sibling) {
            mNodes[oldParent].child1 = newParent;
        } else {
            mNodes[oldParent].child2 = newParent;
        }

        mNodes[newParent].child1 = sibling;
        mNodes[newParent].child2 = leaf;
        mNodes[sibling].parent = newParent;
        mNodes[leaf].parent = newParent;
    } else {
        // The sibling was the root.
        mNodes[newParent].child1 = sibling;
        mNodes[newParent].child2 = leaf;
        mNodes[sibling].parent = newParent;
        mNodes[leaf].parent = newParent;
        mRoot = newParent;
    }

    // Walk back up the tree fixing heights and AABBs
    index = mNodes[leaf].parent;
    while (index != -1) {
        index = Balance(index);

        int32_t child1 = mNodes[index].child1;
        int32_t child2 = mNodes[index].child2;

        assert(child1 != -1);
        assert(child2 != -1);

        mNodes[index].height = 1 + (glm::max)(mNodes[child1].height, mNodes[child2].height);
        mNodes[index].bounds = mNodes[child1].bounds + mNodes[child2].bounds;

        index = mNodes[index].parent;
    }
}

void DynamicTree::RemoveLeaf(int32_t leaf) {
    if (leaf == mRoot) {
        mRoot = -1;
        return;
    }

    int32_t parent = mNodes[leaf].parent;
    int32_t grandParent = mNodes[parent].parent;
    int32_t sibling;
    if (mNodes[parent].child1 == leaf) {
        sibling = mNodes[parent].child2;
    } else {
        sibling = mNodes[parent].child1;
    }

    if (grandParent != -1) {
        // Destroy parent and connect sibling to grandParent.
        if (mNodes[grandParent].child1 == parent) {
            mNodes[grandParent].child1 = sibling;
        } else {
            mNodes[grandParent].child2 = sibling;
        }
        mNodes[sibling].parent = grandParent;
        FreeNode(parent);

        // Adjust ancestor bounds.
        int32_t index = grandParent;
        while (index != -1) {
            index = Balance(index);

            int32_t child1 = mNodes[index].child1;
            int32_t child2 = mNodes[index].child2;

            mNodes[index].bounds = mNodes[child1].bounds + mNodes[child2].bounds;
            mNodes[index].height = 1 + (glm::max)(mNodes[child1].height, mNodes[child2].height);

            index = mNodes[index].parent;
        }
    } else {
        mRoot = sibling;
        mNodes[sibling].parent = -1;
        FreeNode(parent);
    }
}

int32_t DynamicTree::Balance(int32_t iA) {
    assert(iA != -1);

    DynamicTreeNode* A = mNodes + iA;
    if (A->IsLeaf() || A->height < 2)  {
        return iA;
    }

    int32_t iB = A->child1;
    int32_t iC = A->child2;
    assert(0 <= iB && iB < mNodeCapacity);
    assert(0 <= iC && iC < mNodeCapacity);

    DynamicTreeNode* B = mNodes + iB;
    DynamicTreeNode* C = mNodes + iC;

    int32_t balance = C->height - B->height;

    // Rotate C up
    if (balance > 1) {
        int32_t iF = C->child1;
        int32_t iG = C->child2;
        DynamicTreeNode* F = mNodes + iF;
        DynamicTreeNode* G = mNodes + iG;
        assert(0 <= iF && iF < mNodeCapacity);
        assert(0 <= iG && iG < mNodeCapacity);

        // Swap A and C
        C->child1 = iA;
        C->parent = A->parent;
        A->parent = iC;

        // A's old parent should point to C
        if (C->parent != -1) {
            if (mNodes[C->parent].child1 == iA) {
                mNodes[C->parent].child1 = iC;
            } else {
                assert(mNodes[C->parent].child2 == iA);
                mNodes[C->parent].child2 = iC;
            }
        } else {
            mRoot = iC;
        }

        // Rotate
        if (F->height > G->height) {
            C->child2 = iF;
            A->child2 = iG;
            G->parent = iA;
            A->bounds = B->bounds + G->bounds;
            C->bounds = A->bounds + F->bounds;

            A->height = 1 + (glm::max)(B->height, G->height);
            C->height = 1 + (glm::max)(A->height, F->height);
        } else {
            C->child2 = iG;
            A->child2 = iF;
            F->parent = iA;
            A->bounds = B->bounds + F->bounds;
            C->bounds = A->bounds + G->bounds;

            A->height = 1 + (glm::max)(B->height, F->height);
            C->height = 1 + (glm::max)(A->height, G->height);
        }

        return iC;
    }

    // Rotate B up
    if (balance < -1) {
        int32_t iD = B->child1;
        int32_t iE = B->child2;
        DynamicTreeNode* D = mNodes + iD;
        DynamicTreeNode* E = mNodes + iE;
        assert(0 <= iD && iD < mNodeCapacity);
        assert(0 <= iE && iE < mNodeCapacity);

        // Swap A and B
        B->child1 = iA;
        B->parent = A->parent;
        A->parent = iB;

        // A's old parent should point to B
        if (B->parent != -1) {
            if (mNodes[B->parent].child1 == iA) {
                mNodes[B->parent].child1 = iB;
            }
            else {
                assert(mNodes[B->parent].child2 == iA);
                mNodes[B->parent].child2 = iB;
            }
        }
        else {
            mRoot = iB;
        }

        // Rotate
        if (D->height > E->height) {
            B->child2 = iD;
            A->child1 = iE;
            E->parent = iA;
            A->bounds = C->bounds + E->bounds;
            B->bounds = A->bounds + D->bounds;

            A->height = 1 + (glm::max)(C->height, E->height);
            B->height = 1 + (glm::max)(A->height, D->height);
        }
        else {
            B->child2 = iE;
            A->child1 = iD;
            D->parent = iA;
            A->bounds = C->bounds + D->bounds;
            B->bounds = A->bounds + E->bounds;

            A->height = 1 + (glm::max)(C->height, D->height);
            B->height = 1 + (glm::max)(A->height, E->height);
        }

        return iB;
    }

    return iA;
}

int32_t DynamicTree::ComputeHeight() const {
    return ComputeHeight(mRoot);
}

int32_t DynamicTree::ComputeHeight(int32_t nodeId) const {
    assert(0 <= nodeId && nodeId < mNodeCapacity);
    DynamicTreeNode* node = mNodes + nodeId;

    if (node->IsLeaf()) {
        return 0;
    }

    int32_t height1 = ComputeHeight(node->child1);
    int32_t height2 = ComputeHeight(node->child2);
    return 1 + (glm::max)(height1, height2);
}

void DynamicTree::ValidateStructure(int32_t index) const {
    if (index == -1) {
        return;
    }

    if (index == mRoot) {
        assert(mNodes[index].parent == -1);
    }

    const DynamicTreeNode* node = mNodes + index;

    int32_t child1 = node->child1;
    int32_t child2 = node->child2;

    if (node->IsLeaf()) {
        assert(child1 == -1);
        assert(child2 == -1);
        assert(node->height == 0);
        return;
    }

    assert(0 <= child1 && child1 < mNodeCapacity);
    assert(0 <= child2 && child2 < mNodeCapacity);

    assert(mNodes[child1].parent == index);
    assert(mNodes[child2].parent == index);

    ValidateStructure(child1);
    ValidateStructure(child2);
}

void DynamicTree::ValidateMetrics(int32_t index) const {
    if (index == -1) {
        return;
    }

    const DynamicTreeNode* node = mNodes + index;

    int32_t child1 = node->child1;
    int32_t child2 = node->child2;

    if (node->IsLeaf()) {
        assert(child1 == -1);
        assert(child2 == -1);
        assert(node->height == 0);
        return;
    }

    assert(0 <= child1 && child1 < mNodeCapacity);
    assert(0 <= child2 && child2 < mNodeCapacity);

    int32_t height1 = mNodes[child1].height;
    int32_t height2 = mNodes[child2].height;
    int32_t height;
    height = 1 + (glm::max)(height1, height2);
    assert(node->height == height);

    Bounds aabb = mNodes[child1].bounds + mNodes[child2].bounds;

    assert(aabb.lowerBound == node->bounds.lowerBound);
    assert(aabb.upperBound == node->bounds.upperBound);

    ValidateMetrics(child1);
    ValidateMetrics(child2);
}

/*
bool DynamicTree::RayCast(Body *body, const Ray &ray, RayCastHit &hit) const {
    glm::vec3 rayDirection = ray.point2 - ray.point1;
    float tMin = std::numeric_limits<float>::max();
    float tMax = std::numeric_limits<float>::min();
    glm::vec3 normalDirection(0.0f);
    glm::vec3 currentNormal;
    glm::vec3 extents = body->GetBounds().GetExtents() + body->GetBounds().GetCenter();
    for(int i = 0; i < 3; i++) {
        if(glm::abs(rayDirection[i]) < std::numeric_limits<float>::epsilon()) {
            if(ray.point1[i] > extents[i] || ray.point1[i] < -extents[i]) return false;
        } else {
            float oneOverD = 1.0f/rayDirection[i];
            float t1 = (-extents[i] - ray.point1[i])*oneOverD;
            float t2 = ( extents[i] - ray.point1[i])*oneOverD;
            currentNormal[0] = (i == 0) ? -extents[i] : 0.0f;
            currentNormal[1] = (i == 1) ? -extents[i] : 0.0f;
            currentNormal[2] = (i == 2) ? -extents[i] : 0.0f;

            if(t1 > t2) {
                std::swap(t1, t2);
                currentNormal = -currentNormal;
            }

            if(t1 > tMin) {
                tMin = t1;
                normalDirection = currentNormal;
            }
            tMax = (glm::min)(tMax, t2);
            if (tMin > ray.maxFraction) return false;
            if (tMin > tMax) return false;
        }
    }

    if (tMin < 0.0f || tMin > ray.maxFraction) return false;
    glm::vec3 localHitPoint = ray.point1 + tMin * rayDirection;
    hit.body = body;
    hit.hitFraction = tMin;
    hit.point = localHitPoint;
    hit.normal = normalDirection;
    return true;
}
*/