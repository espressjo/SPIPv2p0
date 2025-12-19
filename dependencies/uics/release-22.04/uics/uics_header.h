#ifndef UICS_HEADER_H
#define UICS_HEADER_H
#include <fitsio.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <variant>

// Enum for binary table column types
enum class ColType { DOUBLE, INT, STRING };

// Binary table class
class BinaryTable {
public:
    BinaryTable();
    
    // Create table structure with column names and types
    void create(const std::vector<std::string>& col_names, 
                const std::vector<ColType>& col_types);
    
    // Append a row of data (variadic template for arbitrary columns)
    template<typename... Args>
    void append(Args... args);
    
    // Write the table to a FITS file
    int write_to_fits(fitsfile *fptr) const;
    
    // Get table info
    size_t num_rows() const { return num_rows_; }
    size_t num_cols() const { return col_names_.size(); }
    bool is_created() const { return is_created_; }
    
    // Clear all data (keeps structure)
    void clear_data();
    
private:
    bool is_created_;
    size_t num_rows_;
    std::vector<std::string> col_names_;
    std::vector<ColType> col_types_;
    
    // Storage: one vector per column, using variant for type flexibility
    std::vector<std::variant<std::vector<double>, std::vector<long>, std::vector<std::string>>> columns_;
    
    // Helper to append single value
    template<typename T>
    void append_single(size_t col_idx, T value);
    
    // Recursive helper for variadic append
    template<typename T, typename... Rest>
    void append_impl(size_t col_idx, T first, Rest... rest);
    
    void append_impl(size_t col_idx) {} // Base case
};

class uicsHeader{
    public:
        uicsHeader();
        void add_entry(std::string Kw,std::string value,std::string description);
        void add_entry(std::string Kw,int value,std::string description);
        void add_entry(std::string Kw,double value,std::string description);
        void add_bool_entry(std::string Kw,bool value,std::string description);

        void edit_entry(std::string entry,std::string value);
        void edit_bool_entry(std::string entry,bool value);
        void edit_entry(std::string entry,double value);
        void edit_entry(std::string entry,int value);

        // Get methods to retrieve keyword values
        std::string get_string(std::string kw) const;
        int get_int(std::string kw) const;
        double get_double(std::string kw) const;
        bool get_bool(std::string kw) const;
        
        // Generic get that returns string representation
        std::string get(std::string kw) const;

        int create_header_in_chu(fitsfile *fptr);
        int read_header_from_chu(fitsfile *fptr);
        
        void add_posemeter(std::string ttime, double fflux);
        void add_posemeter(std::string ttime, double fflux,double fflux_optional);
        void add_posemeter(double ttime, double fflux,double fflux_optional);
        int create_table(fitsfile *fptr);
        int posemeter_size;
        
        // Binary table support
        BinaryTable binTable;

    private:
        std::vector<int> intVal;
        std::vector<std::string> intDescription;
        std::vector<std::string> intKW;

        std::vector<int> boolVal;
        std::vector<std::string> boolDescription;
        std::vector<std::string> boolKW;

        std::vector<double> doubleVal;
        std::vector<std::string> doubleDescription;
        std::vector<std::string> doubleKW;

        std::vector<std::string> stringVal;
        std::vector<std::string> stringDescription;
        std::vector<std::string> stringKW;
        std::vector<std::string> time;
        std::vector<double> time_db;
        std::vector<double> flux;
        std::vector<double> flux_optional;
        char** to_array(std::vector<std::string> V);
        double *to_array(std::vector<double> V);
};

// Template implementations must be in header
template<typename... Args>
void BinaryTable::append(Args... args) {
    if (!is_created_) {
        throw std::runtime_error("BinaryTable must be created before appending data");
    }
    
    if (sizeof...(args) != col_names_.size()) {
        throw std::runtime_error("Number of arguments (" + std::to_string(sizeof...(args)) + 
                               ") doesn't match number of columns (" + 
                               std::to_string(col_names_.size()) + ")");
    }
    
    append_impl(0, args...);
    num_rows_++;
}

template<typename T>
void BinaryTable::append_single(size_t col_idx, T value) {
    if (col_idx >= col_types_.size()) {
        throw std::runtime_error("Column index out of range");
    }
    
    ColType expected_type = col_types_[col_idx];
    
    if constexpr (std::is_floating_point_v<T>) {
        if (expected_type != ColType::DOUBLE) {
            throw std::runtime_error("Type mismatch: expected double for column " + 
                                   std::to_string(col_idx));
        }
        std::get<std::vector<double>>(columns_[col_idx]).push_back(static_cast<double>(value));
    } else if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>) {
        if (expected_type != ColType::INT) {
            throw std::runtime_error("Type mismatch: expected int for column " + 
                                   std::to_string(col_idx));
        }
        std::get<std::vector<long>>(columns_[col_idx]).push_back(static_cast<long>(value));
    } else if constexpr (std::is_convertible_v<T, std::string> || std::is_same_v<T, const char*>) {
        if (expected_type != ColType::STRING) {
            throw std::runtime_error("Type mismatch: expected string for column " + 
                                   std::to_string(col_idx));
        }
        std::get<std::vector<std::string>>(columns_[col_idx]).push_back(std::string(value));
    } else {
        throw std::runtime_error("Unsupported type for column " + std::to_string(col_idx));
    }
}

template<typename T, typename... Rest>
void BinaryTable::append_impl(size_t col_idx, T first, Rest... rest) {
    append_single(col_idx, first);
    append_impl(col_idx + 1, rest...);
}

#endif
