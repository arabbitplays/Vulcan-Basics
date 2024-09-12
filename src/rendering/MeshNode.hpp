//
// Created by oster on 12.09.2024.
//

#ifndef BASICS_MESHNODE_HPP
#define BASICS_MESHNODE_HPP


#include "Node.hpp"
#include "../builders/MeshAssetBuilder.hpp"

class MeshNode : public Node {
    std::shared_ptr<MeshAsset> meshAsset;

    virtual void draw(const glm::mat4 topMatrix, DrawContext& ctx);
};


#endif //BASICS_MESHNODE_HPP
