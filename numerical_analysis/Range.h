#pragma once

class FakeIter
{
    using _VType =  size_t;  //��ֵ������Ϊlong����Ȼ�ˣ�Ҳ����д��ģ�����
public:
    explicit FakeIter(_VType val)
        :value_(val) {}
    ~FakeIter() {}

    bool operator != (const FakeIter& other) const { return (this->GetValue()) != (other.GetValue()); }
    _VType operator* () const { return GetValue(); }
    const FakeIter& operator++ () { ++value_; return *this; }

private:
    _VType GetValue() const { return value_; }
    _VType value_;
};

class Range
{
    typedef size_t _VType;    //ͬ����Ҳ����Ū��ģ����������ǾͲ���������
public:
    Range(_VType begin_v, _VType end_v)
        :begin_value_(begin_v), end_value_(end_v) {}
    ~Range() {}

    FakeIter begin() const { return FakeIter(begin_value_); }
    FakeIter end() const { return FakeIter(end_value_); }
private:
    _VType begin_value_;
    _VType end_value_;
};