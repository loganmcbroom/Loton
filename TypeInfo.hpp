//#pragma once

//#include <memory>
//#include <vector>
//#include <nodes/NodeData>

//#include "NodeDataTypes/AudioData.hpp"
//#include "NodeDataTypes/RealFuncData.hpp"
//#include "NodeDataTypes/PVOCData.hpp"

//template<typename  ... Ts>
//std::vector<QtNodes::NodeDataType> make_type_list()
//	{
//	return { Ts::Type() ... };
//	}

//template<typename T> T & dynamic_node_cast( std::shared_ptr<QtNodes::NodeData> ){}

//template<>
//flan::Audio & dynamic_node_cast<flan::Audio>( std::shared_ptr<QtNodes::NodeData> in )
//	{
//	return std::dynamic_pointer_cast<AudioData>( in )->audio;
//	}

//template<>
//flan::PVOC & dynamic_node_cast<flan::PVOC>( std::shared_ptr<QtNodes::NodeData> in )
//	{
//	return std::dynamic_pointer_cast<PVOCData>( in )->flan;
//	}

//template<>
//flan::RealFunc & dynamic_node_cast<flan::RealFunc>( std::shared_ptr<QtNodes::NodeData> in )
//	{
//	return std::dynamic_pointer_cast<RealFuncData>( in )->f;
//	}

////I dare you to combine these into one variadic template
////These take some number of shared_ptr to Base and dynamic casts them all to the types sent in the template args
//template<typename A, typename Base>
//std::tuple< std::shared_ptr<A> >
//dynamic_list_cast( std::vector<std::shared_ptr<Base>> bases )
//	{
//	return {
//		std::dynamic_pointer_cast<A>( bases[0] )
//		};
//	}

//template<typename A, typename B, typename Base>
//std::tuple< std::shared_ptr<A>, std::shared_ptr<B> >
//dynamic_list_cast( std::vector<std::shared_ptr<Base>> bases )
//	{
//	return
//		{ std::dynamic_pointer_cast<A>( bases[0] )
//		, std::dynamic_pointer_cast<B>( bases[1] )
//		};
//	}

//template<typename A, typename B, typename C, typename Base>
//std::tuple< std::shared_ptr<A>, std::shared_ptr<B>, std::shared_ptr<C> >
//dynamic_list_cast( std::vector<std::shared_ptr<Base>> bases )
//	{
//	return
//		{ std::dynamic_pointer_cast<A>( bases[0] )
//		, std::dynamic_pointer_cast<B>( bases[1] )
//		, std::dynamic_pointer_cast<C>( bases[2] )
//		};
//	}

//template<typename A, typename B, typename C, typename D, typename Base>
//std::tuple< std::shared_ptr<A>, std::shared_ptr<B>, std::shared_ptr<C>, std::shared_ptr<D> >
//dynamic_list_cast( std::vector<std::shared_ptr<Base>> bases )
//	{
//	return
//		{ std::dynamic_pointer_cast<A>( bases[0] )
//		, std::dynamic_pointer_cast<B>( bases[1] )
//		, std::dynamic_pointer_cast<C>( bases[2] )
//		, std::dynamic_pointer_cast<D>( bases[3] )
//		};
//	}












