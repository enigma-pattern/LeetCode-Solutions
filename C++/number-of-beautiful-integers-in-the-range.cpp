// Time:  O(n^2 * k), n = len(str(high))
// Space: O(n^2 * k)

// memoization (faster but more space)
class Solution {
public:
    int numberOfBeautifulIntegers(int low, int high, int k) {
        const auto& to_digits = [](int x) {
            vector<int> result;
            for (; x; x /= 10) {
                result.emplace_back(x % 10);
            }
            reverse(begin(result), end(result));
            return result;
        };

        const auto& f = [&](int x) {
            const auto& digits = to_digits(x);
            vector<vector<vector<vector<vector<int>>>>> lookup(size(digits), vector<vector<vector<vector<int>>>>(2, vector<vector<vector<int>>>(2, vector<vector<int>>(2 * size(digits) + 1, vector<int>(k, -1)))));
            function<int (int, int, int, int, int)> memoization = [&](int i, int zero, int tight, int diff, int total) {
                if (i == size(digits)) {
                    return static_cast<int>(zero == 0 && diff == size(digits) && total == 0);
                }
                if (lookup[i][zero][tight][diff][total] == -1) {
                    int result = 0;
                    for (int d = 0; d <= (tight ? digits[i] : 9); ++d) {
                        const int new_zero = static_cast<int>(zero && d == 0);
                        const int new_tight = static_cast<int>(tight && d == digits[i]);
                        const int new_diff = diff + (new_zero == 0 ? (d % 2 == 0 ? 1 : -1): 0);
                        const int new_total = (total * 10 + d) % k;
                        result += memoization(i + 1, new_zero, new_tight, new_diff, new_total);
                    }
                    lookup[i][zero][tight][diff][total] = result;
                }
                return lookup[i][zero][tight][diff][total];
            };
    
            return memoization(0, 1, 1, size(digits), 0);
        };

        return f(high) - f(low - 1);
    }
};

// Time:  O(n^2 * k), n = len(str(high))
// Space: O(n * k)
// dp (slower but less space)
class Solution2 {
public:
    int numberOfBeautifulIntegers(int low, int high, int k) {
        const auto& to_digits = [](int x) {
            vector<int> result;
            for (; x; x /= 10) {
                result.emplace_back(x % 10);
            }
            reverse(begin(result), end(result));
            return result;
        };

        const auto& f = [&](int x) {
            const auto& digits = to_digits(x);
            vector<vector<vector<vector<int64_t>>>> dp(2, vector<vector<vector<int64_t>>>(2, vector<vector<int64_t>>(2 * size(digits) + 1, vector<int64_t>(k))));
            for (int tight = 0; tight < 2; ++tight) {
                dp[0][tight][size(digits)][0] = 1;
            }
            for (int i = size(digits) - 1; i >= 0; --i) {
                vector<vector<vector<vector<int64_t>>>> new_dp(2, vector<vector<vector<int64_t>>>(2, vector<vector<int64_t>>(2 * size(digits) + 1, vector<int64_t>(k))));
                for (int zero = 0; zero < 2; ++zero) {
                    for (int tight = 0; tight < 2; ++tight) {
                        for (int d = 0; d <= (tight ? digits[i] : 9); ++d) {
                            const int new_zero = static_cast<int>(zero && d == 0);
                            const int new_tight = static_cast<int>(tight && d == digits[i]);
                            for (int diff = 0; diff < 2 * size(digits) + 1; ++diff) {
                                const int new_diff = diff + (new_zero == 0 ? (d % 2 == 0 ? 1 : -1): 0);
                                if (!(0 <= new_diff && new_diff < 2 * size(digits) + 1)) {
                                    continue;
                                }
                                for (int total = 0; total < k; ++total) {
                                    const int new_total = (total * 10 + d) % k;
                                    new_dp[zero][tight][diff][total] += dp[new_zero][new_tight][new_diff][new_total];
                                }
                            }
                        }
                    }
                }
                dp = move(new_dp);
            }
            return dp[1][1][size(digits)][0];
        };

        return f(high) - f(low - 1);
    }
};
