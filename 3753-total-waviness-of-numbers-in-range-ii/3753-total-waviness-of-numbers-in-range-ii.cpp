class Solution {
private:
    struct TupleHash {
        template <typename... T>
        std::size_t operator()(const std::tuple<T...>& t) const {
            return apply([](const auto&... args) {
                std::size_t seed = 0;
                ((seed ^= std::hash<std::decay_t<decltype(args)>>{}(args) +
                        0x9e3779b9 + (seed << 6) + (seed >> 2)), ...);
                return seed;
            }, t);
        }
    };

public:
    long long totalWaviness(long long num1, long long num2) {
        const auto& count = [&](long long x) {
            const auto& s = to_string(x);
            using State = tuple<int, int, int, int>;
            unordered_map<State, pair<long long, long long>, TupleHash> dp, new_dp;
            for (int prev = -1; prev <= 9; ++prev) {
                for (int prev2 = -1; prev2 <= 9; ++prev2) {
                    for (int zero = 0; zero <= 1; ++zero) {
                        for (int tight = 0; tight <= 1; ++tight) {
                            dp[{prev, prev2, zero, tight}] = {1, 0};
                        }
                    }
                }
            }
            for (int i = size(s) - 1; i >= 0; --i) {
                new_dp.clear();
                for (int prev = -1; prev <= 9; ++prev) {
                    for (int prev2 = -1; prev2 <= 9; ++prev2) {
                        for (int zero = 0; zero <= 1; ++zero) {
                            for (int tight = 0; tight <= 1; ++tight) {
                                long long cnt = 0, w = 0;
                                const auto& mx = tight ? s[i] - '0' : 9;
                                for (int d = 0; d <= mx; ++d) {
                                    const auto& new_tight = tight && (d == s[i] - '0');
                                    const auto& new_zero = zero && (d == 0);
                                    const auto& new_prev2 = prev;
                                    const auto& new_prev = !new_zero ? d : -1;
                                    State key = {new_prev, new_prev2, new_zero, new_tight};
                                    if (dp.count(key)) {
                                        const auto& [new_cnt, nw] = dp[key];
                                        cnt += new_cnt;
                                        if (!zero && prev2 != -1 && ((prev2 < prev && prev > d) || (prev2 > prev && prev < d))) {
                                            w += new_cnt;
                                        }
                                        w += nw;
                                    }
                                }
                                new_dp[{prev, prev2, zero, tight}] = {cnt, w};
                            }
                        }
                    }
                }
                swap(dp, new_dp);
            }

            return dp[{-1, -1, true, true}].second;
        };

        return count(num2) - count(num1 - 1);
    }
};