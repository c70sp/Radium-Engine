#include "meshLoader.hpp"

bool MeshLoader::loadModel(const std::string& filepath){
    scene = importer.ReadFile(filepath,
        aiProcess_Triangulate  |
        aiProcess_GenNormals |
        //aiProcess_FlipUVs | // Well shit, blender exports with already flipped UVs, so this is unneeded for now
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices
    );

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cerr << "ASSIMP error: " << importer.GetErrorString() << std::endl;
        return false;
    }

    std::cout << "Model loaded successfully" << std::endl;
    std::cout << "Number of meshes: " << scene->mNumMeshes << std::endl;

    return true;
}

std::vector<AssimpMesh> MeshLoader::extractMeshes(){
    std::vector<AssimpMesh> meshes;

    for(unsigned int i = 0; i < scene->mNumMeshes; i++){
        aiMesh* aiMesh = scene->mMeshes[i];
        AssimpMesh mesh;

        mesh.name = aiMesh->mName.C_Str();
        
        std::cout << "Processing mesh: " << i << " : " << aiMesh->mName.C_Str() << std::endl;
        std::cout << "  Vertices: " << aiMesh->mNumVertices << std::endl;
        std::cout << "  Faces: " << aiMesh->mNumFaces << std::endl;
    
        for(unsigned int j = 0; j < aiMesh->mNumVertices; j++){
            mesh.vertices.push_back(aiMesh->mVertices[j].x);
            mesh.vertices.push_back(aiMesh->mVertices[j].y);
            mesh.vertices.push_back(aiMesh->mVertices[j].z);

            if(aiMesh->HasNormals()){
                mesh.normals.push_back(aiMesh->mNormals[j].x);
                mesh.normals.push_back(aiMesh->mNormals[j].y);
                mesh.normals.push_back(aiMesh->mNormals[j].z);
            }

            if(aiMesh->HasTextureCoords(0)){
                mesh.texCoords.push_back(aiMesh->mTextureCoords[0][j].x);
                mesh.texCoords.push_back(aiMesh->mTextureCoords[0][j].y);
            }else{
                mesh.texCoords.push_back(0.0f);
                mesh.texCoords.push_back(0.0f);
            }
        }

        for(unsigned int j = 0; j < aiMesh->mNumFaces; j++){
            aiFace face = aiMesh->mFaces[j];
            for(unsigned int k = 0; k < face.mNumIndices; k++){
                mesh.indices.push_back(face.mIndices[k]);
            }
        }

        meshes.push_back(mesh);
    }

    return meshes;
}

void MeshLoader::printMeshInfo(const AssimpMesh& mesh, int meshIndex){
    std::cout << "\nMesh " << meshIndex << " Information:" << std::endl;
    std::cout << "  Vertices count: " << mesh.vertices.size() / 3 << std::endl;
    std::cout << "  Normals count: " << mesh.normals.size() / 3 << std::endl;
    std::cout << "  Texture coordinates count: " << mesh.texCoords.size() / 2 << std::endl;
    std::cout << "  Indices count: " << mesh.indices.size() << std::endl;
    
    // Print first few vertices as example
    std::cout << "  First 3 vertices:" << std::endl;
    for (int i = 0; i < 3 && i < mesh.vertices.size() / 3; i++) {
        std::cout << "    Vertex " << i << ": (" 
                    << mesh.vertices[i*3] << ", " 
                    << mesh.vertices[i*3+1] << ", " 
                    << mesh.vertices[i*3+2] << ")" << std::endl;
    }
    
    // Print first few indices as example
    std::cout << "  First 9 indices:" << std::endl;
    for (int i = 0; i < 9 && i < mesh.indices.size(); i++) {
        std::cout << mesh.indices[i] << " ";
    }
    std::cout << std::endl;
}

void MeshLoader::getMeshInfo(){

}