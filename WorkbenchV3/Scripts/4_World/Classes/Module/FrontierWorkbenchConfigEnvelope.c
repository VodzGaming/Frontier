class FrontierWorkbenchConfigEnvelope
{
    ref ModuleFrontierPowerConfig power;
    ref FrontierRefreshConfig refresh;
    // Admin and territory radius are configured elsewhere

    void FrontierWorkbenchConfigEnvelope()
    {
        power = new ModuleFrontierPowerConfig();
        refresh = new FrontierRefreshConfig();
    }
}
