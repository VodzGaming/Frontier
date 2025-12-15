class FBCraftQueueCard
{
    protected Widget m_Root;
    protected TextWidget m_Name;
    protected TextWidget m_Timer;
    protected TextWidget m_Player;

    Widget Init(Widget parent, FB_QueueEntry entry, float waitTime = -1)
    {
        m_Root = GetGame().GetWorkspace().CreateWidgets("WorkbenchV3/UI/QueueItem.layout", parent);
        m_Name = TextWidget.Cast(m_Root.FindAnyWidget("QueueRecipeName"));
        m_Timer = TextWidget.Cast(m_Root.FindAnyWidget("QueueTimer"));
        m_Player = TextWidget.Cast(m_Root.FindAnyWidget("QueueRequestedBy"));
        Update(entry, waitTime);
        return m_Root;
    }

    void Update(FB_QueueEntry entry, float waitTime = -1)
    {
        if (!entry)
            return;

        if (m_Name)
        {
            string label = string.Format("%1. %2", entry.Position + 1, entry.RecipeName);
            if (entry.Active)
                label = string.Format("%1 (In Progress)", label);
            m_Name.SetText(label);
        }

        if (m_Timer)
        {
            if (entry.Active)
            {
                string current = FormatTime(entry.RemainingTime);
                string total = FormatTime(entry.TotalTime);
                m_Timer.SetText(string.Format("Remaining %1 / %2", current, total));
            }
            else
            {
                string startsIn = "Waiting";
                if (waitTime >= 0)
                    startsIn = FormatTime(waitTime);
                m_Timer.SetText(string.Format("Starts in %1", startsIn));
            }
        }

        if (m_Player)
        {
            if (entry.RequestedBy && entry.RequestedBy != "")
                m_Player.SetText(entry.RequestedBy);
            else
                m_Player.SetText("-");
        }
    }

    void SetActive(bool active)
    {
        if (!m_Root)
            return;

        if (active)
            m_Root.SetAlpha(1.0);
        else
            m_Root.SetAlpha(0.6);
    }

    void Destroy()
    {
        if (m_Root)
        {
            m_Root.GetParent().RemoveChild(m_Root);
            m_Root = null;
        }
    }

    protected string FormatTime(float seconds)
    {
        if (seconds < 0)
            seconds = 0;

        int total = Math.Floor(seconds);
        int mins = total / 60;
        int secs = total % 60;
        string secStr = secs.ToString();
        if (secs < 10)
            secStr = string.Format("0%1", secStr);
        return string.Format("%1:%2", mins.ToString(), secStr);
    }
}
