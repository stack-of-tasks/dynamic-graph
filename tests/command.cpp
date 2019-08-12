//
// Created by corentin on 7/3/19.
//

#include <boost/foreach.hpp>

#include <dynamic-graph/debug.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/signal-array.h>
#include <dynamic-graph/command-bind.h>
#include <dynamic-graph/tracer.h>

#include <assert.h>
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>


#define BOOST_TEST_MODULE signal_array



using boost::test_tools::output_test_stream;

dynamicgraph::SignalArray_const<double> sig;

using namespace std;
using namespace dynamicgraph;
using namespace dynamicgraph::command;

template <class E >
struct CV0 : public dynamicgraph::command::CommandVoid0<E> {
public:
    CV0(E& entity, boost::function<void(void)> function,
        const std::string& docString)
            : dynamicgraph::command::CommandVoid0<E>(entity, function, docString),test(function)
    {};


    void Exec() {
        dynamicgraph::command::CommandVoid0<E>::doExecute();
    }
private:
    boost::function<void(void)> test;
};

template <class E >
CV0<E>* makeCV0(E& entity, boost::function<void(void)> function,
                const std::string& docString)
{
    return new CV0<E>(entity,function,docString);
}

Tracer::Tracer( const std::string n )
        :Entity(n)
        ,toTraceSignals ()
        ,traceStyle(TRACE_STYLE_DEFAULT)
        ,frequency(1)
        ,basename ()
        ,suffix(".dat")
        ,rootdir ()
        ,namesSet( false )
        ,files ()
        ,names ()
        ,play(false)
        ,timeStart(0)
        ,triger( boost::bind(&Tracer::recordTrigger,this,_1,_2),
                 sotNOSIGNAL,
                 "Tracer("+n+")::triger" )
{
    std::string doc;

    doc = docCommandVoid0("test");
    addCommand("clear",
               makeCV0(*this,&Tracer::clearSignalToTrace,doc ));
}


BOOST_AUTO_TEST_CASE (test) {
    output_test_stream test;
    test << "1";
    BOOST_CHECK(test.is_equal("1"));
}