#ifndef AI_BVHLOADER_H_INC
#define AI_BVHLOADER_H_INC

#include <assimp/BaseImporter.h>

struct aiNode;

namespace Assimp {

// --------------------------------------------------------------------------------
/** Loader class to read Motion Capturing data from a .bvh file.
 *
 * This format only contains a hierarchy of joints and a series of keyframes for
 * the hierarchy. It contains no actual mesh data, but we generate a dummy mesh
 * inside the loader just to be able to see something.
*/
class BVHLoader : public BaseImporter {

    /** Possible animation channels for which the motion data holds the values */
    enum ChannelType {
        Channel_PositionX,
        Channel_PositionY,
        Channel_PositionZ,
        Channel_RotationX,
        Channel_RotationY,
        Channel_RotationZ
    };

    /** Collected list of node. Will be bones of the dummy mesh some day, addressed by their array index */
    struct Node {
        const aiNode *mNode;
        std::vector<ChannelType> mChannels;
        std::vector<float> mChannelValues; // motion data values for that node. Of size NumChannels * NumFrames

        Node() :
                mNode(nullptr) {}

        explicit Node(const aiNode *pNode) :
                mNode(pNode) {}
    };

public:
    BVHLoader();
    ~BVHLoader();

public:
    /** Returns whether the class can handle the format of the given file.
     * See BaseImporter::CanRead() for details. */
    bool CanRead(const std::string &pFile, IOSystem *pIOHandler, bool cs) const;

    void SetupProperties(const Importer *pImp);
    const aiImporterDesc *GetInfo() const;

protected:
    /** Imports the given file into the given scene structure.
     * See BaseImporter::InternReadFile() for details
     */
    void InternReadFile(const std::string &pFile, aiScene *pScene, IOSystem *pIOHandler);

protected:
    /** Reads the file */
    void ReadStructure(aiScene *pScene);

    /** Reads the hierarchy */
    void ReadHierarchy(aiScene *pScene);

    /** Reads a node and recursively its children and returns the created node. */
    aiNode *ReadNode();

    /** Reads an end node and returns the created node. */
    aiNode *ReadEndSite(const std::string &pParentName);

    /** Reads a node offset for the given node */
    void ReadNodeOffset(aiNode *pNode);

    /** Reads the animation channels into the given node */
    void ReadNodeChannels(BVHLoader::Node &pNode);

    /** Reads the motion data */
    void ReadMotion(aiScene *pScene);

    /** Retrieves the next token */
    std::string GetNextToken();

    /** Reads the next token as a float */
    float GetNextTokenAsFloat();

    /** Aborts the file reading with an exception */
    template<typename... T>
    AI_WONT_RETURN void ThrowException(T&&... args) AI_WONT_RETURN_SUFFIX;

    /** Constructs an animation for the motion data and stores it in the given scene */
    void CreateAnimation(aiScene *pScene);

protected:
    /** Filename, for a verbose error message */
    std::string mFileName;

    /** Buffer to hold the loaded file */
    std::vector<char> mBuffer;

    /** Next char to read from the buffer */
    std::vector<char>::const_iterator mReader;

    /** Current line, for error messages */
    unsigned int mLine;

    /** Collected list of nodes. Will be bones of the dummy mesh some day, addressed by their array index.
    * Also contain the motion data for the node's channels
    */
    std::vector<Node> mNodes;

    /** basic Animation parameters */
    float mAnimTickDuration;
    unsigned int mAnimNumFrames;

    bool noSkeletonMesh;
};

} // end of namespace Assimp

#endif // AI_BVHLOADER_H_INC
