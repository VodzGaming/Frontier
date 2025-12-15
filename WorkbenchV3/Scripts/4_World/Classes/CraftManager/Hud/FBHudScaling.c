class FBHudScaling
{
    // Returns a scale factor based on the current screen size using a 4K baseline.
    static float ComputeUIScale(float baseW = 3840.0, float baseH = 2160.0, float minScale = 0.4, float maxScale = 1.0)
    {
        float screenW, screenH;
        GetGame().GetWorkspace().GetScreenSize(screenW, screenH);

        if (baseW <= 0 || baseH <= 0)
            return 1.0;

        float scaleW = screenW / baseW;
        float scaleH = screenH / baseH;
        float scale = Math.Min(scaleW, scaleH);

        if (scale < minScale)
            scale = minScale;
        if (scale > maxScale)
            scale = maxScale;

        return scale;
    }

    static void ApplyTo(Widget target, float baseW = 3840.0, float baseH = 2160.0, float minScale = 0.6, float maxScale = 1.0)
    {
        if (!target)
            return;

        float scale = ComputeUIScale(baseW, baseH, minScale, maxScale);
        float w, h;
        target.GetSize(w, h);
        target.SetSize(w * scale, h * scale);
    }
}
