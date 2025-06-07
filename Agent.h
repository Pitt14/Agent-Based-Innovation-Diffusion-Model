#pragma once

// Agent is an abstraction of an individual in a social network,
// possesing its opinion on a certain topic.
// An opininion is represented as a boolean value.
class Agent
{
public:
    // Constructor to initialize the agent with a specific opinion.
    Agent(bool initial_opinion):
        _opinion(initial_opinion)
    {
    }

    Agent(): _opinion(false){}

    // Returns the current opinion of the agent.
    bool getOpinion() const
    {
        return _opinion;
    }

    // Sets a new opinion for the agent.
    void setOpinion(bool new_opinion)
    {
        _opinion = new_opinion;
    }

    int operator+ (Agent& other)
    {
        return _opinion + other.getOpinion();
    }

    int operator- (Agent& other)
    {
        return _opinion - other.getOpinion();
    }

private:
    bool _opinion;
};

int operator+ (int n, const Agent& agent)
{
    return n + agent.getOpinion();
}

int operator- (int n, const Agent& agent)
{
    return n - agent.getOpinion();
}