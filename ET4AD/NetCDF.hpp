/* 
 * File:   NetCDF.hpp
 * Author: matthewsupernaw
 *
 * Created on August 26, 2014, 11:02 AM
 */

#ifndef NETCDF_HPP
#define	NETCDF_HPP

#include <netcdf>

using namespace netCDF;

class NetCDFException {
public:

    NetCDFException(std::string what) {
        this->what_ = what;
    }

    virtual const char* what() const throw () {
        return this->what_.c_str();
    }

private:
    std::string what_;
};

class Index {
public:

    unsigned int operator[](unsigned int dim) {
        switch (dim) {
            case 0:
                return I;
            case 1:
                return J;
            case 2:
                return K;
            case 3:
                return L;
            case 4:
                return M;
            case 5:
                return N;
            default:
                return I;

        }
    }

    Index(unsigned int i)
    : I(i), J(0), K(0), L(0), M(0), N(0) {
    }

    Index(unsigned int i, unsigned int j)
    : I(i), J(j), K(0), L(0), M(0), N(0) {
    }

    Index(unsigned int i, unsigned int j, unsigned int k)
    : I(i), J(j), K(k), L(0), M(0), N(0) {
    }

    Index(unsigned int i, unsigned int j, unsigned int k,
            unsigned int l)
    : I(i), J(j), K(k), L(l), M(0), N(0) {
    }

    Index(unsigned int i, unsigned int j, unsigned int k,
            unsigned int l, unsigned int m)
    : I(i), J(j), K(k), L(l), M(m), N(0) {
    }

    Index(unsigned int i, unsigned int j,
            unsigned int k, unsigned int l, unsigned int m,
            unsigned int n)
    : I(i), J(j), K(k), L(l), M(m), N(n) {
    }

    unsigned int GetI() const {
        return I;
    }

    void SetI(unsigned int I) {
        this->I = I;
    }

    unsigned int GetJ() const {
        return J;
    }

    void SetJ(unsigned int J) {
        this->J = J;
    }

    unsigned int GetK() const {
        return K;
    }

    void SetK(unsigned int K) {
        this->K = K;
    }

    unsigned int GetL() const {
        return L;
    }

    void SetL(unsigned int L) {
        this->L = L;
    }

    unsigned int GetM() const {
        return M;
    }

    void SetM(unsigned int M) {
        this->M = M;
    }

    unsigned int GetN() const {
        return N;
    }

    void SetN(unsigned int N) {
        this->N = N;
    }

    unsigned int GetCount() const {
        return count;
    }

    void SetCount(unsigned int count) {
        this->count = count;
    }
private:
    unsigned int count;
    unsigned int I;
    unsigned int J;
    unsigned int K;
    unsigned int L;
    unsigned int M;
    unsigned int N; //getting a little crazy...

};

class NetCDFOutput {
public:

    NetCDFOutput() {
    }

    NetCDFOutput(std::string name) {
        this->SetName(name);
        outFile = new NcFile(name.data(), NcFile::Replace, NULL, 0,
                netCDF::NcFile::Offset64Bits);
        outFile->set_fill(NcFile::Fill);

    }

    ~NetCDFOutput() {
    }

    void SetOpen(std::string path) {
        this->SetName(path);
        outFile = new NcFile(name.data(), NcFile::Replace, NULL, 0,
                NcFile::Offset64Bits);
        outFile->set_fill(NcFile::Fill);
    }

    void SetName(std::string name) {
        this->name;
    }

    void AddDimension(std::string name, int length) {
        NcDim* dim = outFile->add_dim(name.c_str(), length);
    }

    void AddUnlimitedDimension(std::string name) {
        NcDim* dim = outFile->add_dim(name.c_str());
    }

    NcDim* GetDimension(std::string name) {
        return this->outFile->get_dim(name.c_str());
    }

    void AddVariable(std::string name, NcType datatype) {
        NcVar* var = outFile->add_var(name.c_str(), datatype);
        if (var == NULL) {
            std::string error = "Null value return after adding variable named " + name + ".";
            throw new NetCDFException(error);
        }
    }

    void AddVariable(std::string name, NcType datatype, NcDim* d1) {
        NcVar* var = outFile->add_var(name.c_str(), datatype, d1);
        if (var == NULL) {
            std::string error = "Null value return after adding variable named " + name + ".";
            throw new NetCDFException(error);
        }
    }

    void AddVariable(std::string name, NcType datatype, NcDim* d1, NcDim* d2) {
        NcVar* var = outFile->add_var(name.c_str(), datatype, d1, d2);
        if (var == NULL) {
            std::string error = "Null value return after adding variable named " + name + ".";
            throw new NetCDFException(error);
        }
    }

    void AddVariable(std::string name, NcType datatype, NcDim* d1, NcDim* d2, NcDim* d3) {
        NcVar* var = outFile->add_var(name.c_str(), datatype, d1, d2, d3);
        if (var == NULL) {
            std::string error = "Null value return after adding variable named " + name + ".";
            throw new NetCDFException(error);
        }
    }

    void AddVariable(std::string name, NcType datatype, NcDim* d1, NcDim* d2, NcDim* d3, NcDim* d4) {
        NcVar* var = outFile->add_var(name.c_str(), datatype, d1, d2, d3, d4);
        if (var == NULL) {
            std::string error = "Null value return after adding variable named " + name + ".";
            throw new NetCDFException(error);
        }
    }

    void AddVariable(std::string name, NcType datatype, NcDim* d1, NcDim* d2, NcDim* d3, NcDim* d4, NcDim* d5) {
        NcVar* var = outFile->add_var(name.c_str(), datatype, d1, d2, d3, d4, d5);
        if (var == NULL) {
            std::string error = "Null value return after adding variable named " + name + ".";
            throw new NetCDFException(error);
        }
    }

    void AddVariable(std::string name, NcType datatype, int rank, const NcDim** d) {
        NcVar* var = outFile->add_var(name.c_str(), datatype, rank, d);
        if (var == NULL) {
            std::string error = "Null value return after adding variable named " + name + ".";
            throw new NetCDFException(error);
        }
    }

    void AddVariableAttribute(std::string variableName, std::string attributeName, ncbyte attribute) {
        NcVar* var = this->outFile->get_var(variableName.c_str());
        var->add_att(attributeName.c_str(), attribute);
    }

    void AddVariableAttribute(std::string variableName, std::string attributeName, char attribute) {
        NcVar* var = this->outFile->get_var(variableName.c_str());
        var->add_att(attributeName.c_str(), attribute);
    }

    void AddVariableAttribute(std::string variableName, std::string attributeName, short attribute) {
        NcVar* var = this->outFile->get_var(variableName.c_str());
        var->add_att(attributeName.c_str(), attribute);
    }

    void AddVariableAttribute(std::string variableName, std::string attributeName, int attribute) {
        NcVar* var = this->outFile->get_var(variableName.c_str());
        var->add_att(attributeName.c_str(), attribute);
    }

    void AddVariableAttribute(std::string variableName, std::string attributeName, long attribute) {
        NcVar* var = this->outFile->get_var(variableName.c_str());
        var->add_att(attributeName.c_str(), attribute);
    }

    void AddVariableAttribute(std::string variableName, std::string attributeName, float attribute) {
        NcVar* var = this->outFile->get_var(variableName.c_str());
        var->add_att(attributeName.c_str(), attribute);
    }

    void AddVariableAttribute(std::string variableName, std::string attributeName, double attribute) {
        NcVar* var = this->outFile->get_var(variableName.c_str());
        var->add_att(attributeName.c_str(), attribute);
    }

    void AddVariableAttribute(std::string variableName, std::string attributeName, std::string attribute) {
        NcVar* var = this->outFile->get_var(variableName.c_str());
        var->add_att(attributeName.c_str(), attribute.c_str());
    }

    NcVar* GetVariable(std::string name) {
        return this->outFile->get_var(name.c_str());
    }

    void AddGlobalAttribute(std::string attributeName, std::string attribute) {
        this->outFile->add_att(attributeName.c_str(), attribute.c_str());
    }

    void Close() {
        this->outFile->close();
    }

    void WriteByte(std::string variable, Index index, ncbyte value) {
        NcVar* var = this->outFile->get_var(variable.c_str());
        int dims = var->num_dims();
        ncbyte data[] = {value};

        switch (dims) {
            case 1:
                var->set_cur((long) index[0]);
                var->put(&data[0], 1);
                break;
            case 2:
                var->set_cur((long) index[0], (long) index[1]);
                var->put(&data[0], 1, 1);
                break;
            case 3:
                var->set_cur((long) index[0], (long) index[1], (long) index[2]);
                var->put(&data[0], 1, 1, 1);
                break;
            case 4:
                var->set_cur((long) index[0], (long) index[1], (long) index[2], (long) index[3]);
                var->put(&data[0], 1, 1, 1, 1);
                break;
            case 5:
                var->set_cur((long) index[0], (long) index[1], (long) index[2], (long) index[3], (long) index[4]);
                var->put(&data[0], 1, 1, 1, 1, 1);
                break;
            default:
                break;
        }

    }

    void WriteShort(std::string variable, Index index, short value) {
        NcVar* var = this->outFile->get_var(variable.c_str());
        int dims = var->num_dims();
        short data[] = {value};

        switch (dims) {
            case 1:
                var->set_cur((long) index[0]);
                var->put(&data[0], 1);
                break;
            case 2:
                var->set_cur((long) index[0], (long) index[1]);
                var->put(&data[0], 1, 1);
                break;
            case 3:
                var->set_cur((long) index[0], (long) index[1], (long) index[2]);
                var->put(&data[0], 1, 1, 1);
                break;
            case 4:
                var->set_cur((long) index[0], (long) index[1], (long) index[2], (long) index[3]);
                var->put(&data[0], 1, 1, 1, 1);
                break;
            case 5:
                var->set_cur((long) index[0], (long) index[1], (long) index[2], (long) index[3], (long) index[4]);
                var->put(&data[0], 1, 1, 1, 1, 1);
                break;
            default:
                break;
        }

    }

    void WriteChar(std::string variable, Index index, char value) {
        NcVar* var = this->outFile->get_var(variable.c_str());
        int dims = var->num_dims();
        char data[] = {value};

        switch (dims) {
            case 1:
                var->set_cur((long) index[0]);
                var->put(&data[0], 1);
                break;
            case 2:
                var->set_cur((long) index[0], (long) index[1]);
                var->put(&data[0], 1, 1);
                break;
            case 3:
                var->set_cur((long) index[0], (long) index[1], (long) index[2]);
                var->put(&data[0], 1, 1, 1);
                break;
            case 4:
                var->set_cur((long) index[0], (long) index[1], (long) index[2], (long) index[3]);
                var->put(&data[0], 1, 1, 1, 1);
                break;
            case 5:
                var->set_cur((long) index[0], (long) index[1], (long) index[2], (long) index[3], (long) index[4]);
                var->put(&data[0], 1, 1, 1, 1, 1);
                break;
            default:
                break;
        }

    }

    void WriteInt(std::string variable, Index index, int value) {
        NcVar* var = this->outFile->get_var(variable.c_str());
        int dims = var->num_dims();
        int data[] = {value};

        switch (dims) {
            case 1:
                var->set_cur((long) index[0]);
                var->put(&data[0], 1);
                break;
            case 2:
                var->set_cur((long) index[0], (long) index[1]);
                var->put(&data[0], 1, 1);
                break;
            case 3:
                var->set_cur((long) index[0], (long) index[1], (long) index[2]);
                var->put(&data[0], 1, 1, 1);
                break;
            case 4:
                var->set_cur((long) index[0], (long) index[1], (long) index[2], (long) index[3]);
                var->put(&data[0], 1, 1, 1, 1);
                break;
            case 5:
                var->set_cur((long) index[0], (long) index[1], (long) index[2], (long) index[3], (long) index[4]);
                var->put(&data[0], 1, 1, 1, 1, 1);
                break;
            default:
                break;
        }

    }

    void WriteLong(std::string variable, Index index, long value) {
        NcVar* var = this->outFile->get_var(variable.c_str());
        int dims = var->num_dims();
        long data[] = {value};

        switch (dims) {
            case 1:
                var->set_cur((long) index[0]);
                var->put(&data[0], 1);
                break;
            case 2:
                var->set_cur((long) index[0], (long) index[1]);
                var->put(&data[0], 1, 1);
                break;
            case 3:
                var->set_cur((long) index[0], (long) index[1], (long) index[2]);
                var->put(&data[0], 1, 1, 1);
                break;
            case 4:
                var->set_cur((long) index[0], (long) index[1], (long) index[2], (long) index[3]);
                var->put(&data[0], 1, 1, 1, 1);
                break;
            case 5:
                var->set_cur((long) index[0], (long) index[1], (long) index[2], (long) index[3], (long) index[4]);
                var->put(&data[0], 1, 1, 1, 1, 1);
                break;
            default:
                break;
        }

    }

    void WriteFloat(std::string variable, Index index, float value) {
        NcVar* var = this->outFile->get_var(variable.c_str());
        int dims = var->num_dims();
        float data[] = {value};

        switch (dims) {
            case 1:
                var->set_cur((long) index[0]);
                var->put(&data[0], 1);
                break;
            case 2:
                var->set_cur((long) index[0], (long) index[1]);
                var->put(&data[0], 1, 1);
                break;
            case 3:
                var->set_cur((long) index[0], (long) index[1], (long) index[2]);
                var->put(&data[0], 1, 1, 1);
                break;
            case 4:
                var->set_cur((long) index[0], (long) index[1], (long) index[2], (long) index[3]);
                var->put(&data[0], 1, 1, 1, 1);
                break;
            case 5:
                var->set_cur((long) index[0], (long) index[1], (long) index[2], (long) index[3], (long) index[4]);
                var->put(&data[0], 1, 1, 1, 1, 1);
                break;
            default:
                break;
        }

    }

    void WriteDouble(std::string variable, Index index, double value) {
        NcVar* var = this->outFile->get_var(variable.c_str());
        int dims = var->num_dims();
        double data[] = {value};

        switch (dims) {
            case 1:
                var->set_cur((long) index[0]);
                var->put(&data[0], 1);
                break;
            case 2:
                var->set_cur((long) index[0], (long) index[1]);
                var->put(&data[0], 1, 1);
                break;
            case 3:
                var->set_cur((long) index[0], (long) index[1], (long) index[2]);
                var->put(&data[0], 1, 1, 1);
                break;
            case 4:
                var->set_cur((long) index[0], (long) index[1], (long) index[2], (long) index[3]);
                var->put(&data[0], 1, 1, 1, 1);
                break;
            case 5:
                var->set_cur((long) index[0], (long) index[1], (long) index[2], (long) index[3], (long) index[4]);
                var->put(&data[0], 1, 1, 1, 1, 1);
                break;
            default:
                break;
        }

    }

private:
    std::string name;
    NcFile* outFile;

};


#endif	/* NETCDF_HPP */

