#pragma once

namespace seam::ast {
    template<typename... Types>
    struct Visitor;

    template<typename T>
    struct Visitor<T>
    {
	    virtual ~Visitor() = default;
	    virtual void visit(T& visitable) = 0;
    };

    template<typename T, typename... Types>
    struct Visitor<T, Types...> : public Visitor<Types...>
    {
        using Visitor<Types...>::visit;

        virtual void visit(T& visitable) = 0;
    };
}