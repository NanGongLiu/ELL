#include "SharedLinearBinaryPredictor.h"

#include "CoordinateListFactory.h"
using layers::Coordinate;
using layers::CoordinateListFactory;

#include "layers.h"
using layers::Scale;
using layers::Sum;
using layers::Shift;

#include <memory>
using std::make_shared;

namespace predictors
{
    SharedLinearBinaryPredictor::BiasedVector::BiasedVector(uint64 dim) : w(dim), b(0.0)
    {}

    SharedLinearBinaryPredictor::SharedLinearBinaryPredictor(uint64 dim)
    {
        _sp_predictor = make_shared<BiasedVector>(dim);
    }

    DoubleVector & SharedLinearBinaryPredictor::GetVector()
    {
        return _sp_predictor->w;
    }

    const DoubleVector & SharedLinearBinaryPredictor::GetVector() const
    {
        return _sp_predictor->w;
    }

    double & SharedLinearBinaryPredictor::GetBias()
    {
        return _sp_predictor->b;
    }

    double SharedLinearBinaryPredictor::GetBias() const
    {
        return _sp_predictor->b;
    }

    void SharedLinearBinaryPredictor::AddTo(Map& map, const CoordinateList& inputCoordinates) const
    {
        uint64 rowIndex = map.PushBack(make_shared<Scale>(_sp_predictor->w, inputCoordinates));

        CoordinateList scaleOutputs = CoordinateListFactory::Sequence(rowIndex, _sp_predictor->w.Size());
        rowIndex = map.PushBack(make_shared<Sum>(scaleOutputs));

        map.PushBack(make_shared<Shift>(_sp_predictor->b, Coordinate{rowIndex, 0}));
    }
}