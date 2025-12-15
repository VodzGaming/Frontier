class FB_PowerConsumer
{
    float m_ConsumptionPerHour;
    void FB_PowerConsumer(float wh){ m_ConsumptionPerHour = wh; }
    float GetConsumption(){ return m_ConsumptionPerHour; }
}