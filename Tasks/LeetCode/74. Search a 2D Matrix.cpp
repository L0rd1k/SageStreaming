class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int left = 0;
        int right = matrix.size() - 1;
        int mid = 0;
        
        if (!matrix.size() || !matrix[0].size()) return false;

        while(left < right) {
            mid = (left + right) / 2;
            if(matrix[mid].back() < target) {
                left = mid + 1;
            } else if (matrix[mid][0] > target) {
                right = mid - 1;
            } else {
                left = mid;
                break;
            }
        }
        
        std::cout << "Check" << std::endl;
        int row = left;
        left = 0;
        right = matrix[0].size() - 1;
        while(left <= right) {
            mid = (left + right) / 2;
            if(matrix[row][mid] < target) {
                left = mid + 1;
            } else if (matrix[row][mid] > target) {
                right = mid - 1;
            } else {
                return true;
            }
        }
        return false;
    }
};