module;
export module bimap;

import std;

export template <typename A, typename B>
class bimap {
   private:
    std::unordered_map<A, B> ab;
    std::unordered_map<B, A> ba;

   public:
    void insert(const A& a, const B& b) {
        ab[a] = b;
        ba[b] = a;
    };

    void remove(const A& a, const B& b) {
        ab.erase(a);
        ba.erase(b);
    };
    void remove(const A& a) {
        B b = getB(a);
        remove(a, b);
    };
    void remove(const B& b) {
        A a = getA(b);
        remove(a, b);
    };

    const A& getA(const B& b) const { return ba.at(b); };
    const B& getB(const A& a) const { return ab.at(a); };
};