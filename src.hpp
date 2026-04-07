#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>

class BaseJudger {
public:
    BaseJudger (size_t time_limit , size_t memory_limit ,
                const char *answer) : time_limit_(time_limit), memory_limit_(memory_limit), score_(0) {
        answer_ = new char[strlen(answer) + 1];
        strcpy(answer_, answer);
    }

    virtual void Submit (size_t time , size_t memory , const char *output) = 0;

    size_t GetScore () const { return score_; }

    virtual ~BaseJudger () {
        delete[] answer_;
    }

protected:
    char *answer_;
    const size_t time_limit_;
    const size_t memory_limit_;
    size_t score_;

    virtual bool CheckAnswer (const char *output) const {
        // the output must equal to the answer
        return strcmp(answer_ , output) == 0;
    }
};

class OIJudger : public BaseJudger {
private:
    bool submitted_;
public:
    OIJudger(size_t time_limit, size_t memory_limit, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer), submitted_(false) {}

    void Submit(size_t time, size_t memory, const char *output) override {
        if (submitted_) {
            score_ = 0;
            return;
        }
        submitted_ = true;
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswer(output)) {
            score_ = 100;
        } else {
            score_ = 0;
        }
    }

protected:
    bool CheckAnswer(const char *output) const override {
        const char* p1 = answer_;
        const char* p2 = output;
        while (true) {
            const char* e1 = p1;
            while (*e1 != '\n' && *e1 != '\0') e1++;
            const char* e2 = p2;
            while (*e2 != '\n' && *e2 != '\0') e2++;
            
            const char* last1 = e1 - 1;
            while (last1 >= p1 && (*last1 == ' ' || *last1 == '\r')) last1--;
            
            const char* last2 = e2 - 1;
            while (last2 >= p2 && (*last2 == ' ' || *last2 == '\r')) last2--;
            
            size_t len1 = last1 - p1 + 1;
            size_t len2 = last2 - p2 + 1;
            if (len1 != len2) return false;
            if (strncmp(p1, p2, len1) != 0) return false;
            
            if (*e1 == '\n' && *e2 == '\n') {
                p1 = e1 + 1;
                p2 = e2 + 1;
            } else if (*e1 == '\0' && *e2 == '\0') {
                break;
            } else {
                return false;
            }
        }
        return true;
    }
};

class ICPCJudger : public BaseJudger {
public:
    ICPCJudger(size_t time_limit, size_t memory_limit, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer) {}

    void Submit(size_t time, size_t memory, const char *output) override {
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswer(output)) {
            score_ = 100;
        }
    }
};

class SpacialJudger : public BaseJudger {
private:
    size_t full_score_time_;
    size_t full_score_memory_;
public:
    SpacialJudger(size_t time_limit, size_t memory_limit, size_t full_score_time, size_t full_score_memory, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer), full_score_time_(full_score_time), full_score_memory_(full_score_memory) {}

    void Submit(size_t time, size_t memory, const char *output) override {
        if (!CheckAnswer(output)) {
            return;
        }
        
        size_t time_score = 0;
        if (time <= full_score_time_) {
            time_score = 100;
        } else if (time >= time_limit_) {
            time_score = 0;
        } else {
            double ts = 100.0 * (time_limit_ - time) / (time_limit_ - full_score_time_);
            time_score = std::floor(ts);
        }
        
        size_t memory_score = 0;
        if (memory <= full_score_memory_) {
            memory_score = 100;
        } else if (memory >= memory_limit_) {
            memory_score = 0;
        } else {
            double ms = 100.0 * (memory_limit_ - memory) / (memory_limit_ - full_score_memory_);
            memory_score = std::floor(ms);
        }
        
        size_t current_score = std::floor(1.0 * time_score * memory_score / 100.0);
        if (current_score > score_) {
            score_ = current_score;
        }
    }
};

