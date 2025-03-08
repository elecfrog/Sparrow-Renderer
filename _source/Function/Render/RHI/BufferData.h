class BufferData
{
public:
    size_t m_size{ 0 };
    void* m_data{ nullptr };

    BufferData() = delete;
    BufferData(size_t size)
    {
        m_size = size;
        m_data = malloc(size);
    }
    ~BufferData()
    {
        if (m_data)
        {
            free(m_data);
        }
    }
    bool isValid() const { return m_data != nullptr; }
};