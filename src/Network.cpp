#include "Network.hpp"

Network::Network() : Network(NetworkInfo("GraphPropertyName","GraphPropertyLocation"))
{
}

Network::Network(NetworkInfo n) : network_graph(0,n), vertex_list(), vertex_exist(), edge_list()
{
  dp = boost::dynamic_properties(boost::ignore_other_properties);

  dp.property("label", boost::get(&Routeur::name, network_graph));
  dp.property("is_multicast", boost::get(&Routeur::is_multicast, network_graph));

  dp.property("label", boost::get(&Cable::length, network_graph));
}

Network::~Network()
{

}

std::string& Network::add_routeur()
{
  static unsigned int id_count = 0;
  Routeur r;
  r.name = std::to_string(id_count);
  r.is_multicast=default_routeur_is_multicast;
  vertex_t v_desc = add_vertex(r,network_graph);
  vertex_list[r.name] = v_desc;
  id_count++;
  return r.name;
}

int Network::add_routeur(std::string& name)
{
  Routeur r;
  r.name = name;
  r.is_multicast=default_routeur_is_multicast;
  vertex_t v_desc = add_vertex(r,network_graph);
  vertex_list[name] = v_desc;
  return 0;
}

int Network::add_cable(std::string& id1, std::string& id2)
{
  unsigned int nb_vert = vertex_list.size();

  // to fix
  if((id1>=nb_vert||id2>=nb_vert))
    {
      std::cerr << "Routeur " << id1 << " or " << id2 << " does not exist." << std::endl;
      return -1;
    }
  if(id1==id2)
    {
      std::cerr << "Trying to link routeur " << id1 << "with itself; reflexivity is forbidden" << std::endl;
      return -1;
    }

  Cable c;
  c.length=default_cable_length;

  auto tmp1 = add_edge(vertex_list[id1], vertex_list[id2], c, network_graph);
  edge_list.push_back(tmp1.first);
  auto tmp2 = add_edge(vertex_list[id2], vertex_list[id1], c, network_graph);
  //todo rder by name ? or id ?
  edge_listtmp2.first;

  return (tmp1.second==false||tmp2.second==false)?0:-1;
}

template <typename Structure, typename Attribute>
Attribute& Network::get_attribute(std::string& id)
{
  return boost::get(&Structure::Attribute,network_graph,vertex_list[id]);
}

template <typename Structure, typename Attribute>
void Network::set_attribute(std::string& id, Attribute value)
{
  boost::put(&Structure::Attribute,network_graph,vertex_list[id], value);
}

int Network::remove_routeur(std::string&)
{
  return 0;
}

int Network::remove_cable(std::string&)
{
  return 0;
}

std::vector<std::string> Network::get_path(std::string &source, std::string &destination)
{
    vertex_t start_node = vertex_list[source];
    vertex_t end_node = vertex_list[destination];

    IndexMap id_map = boost::get(boost::vertex_index,network_graph);

    std::vector<vertex_t> predecessors(boost::num_vertices(network_graph));
    std::vector<int> distances(boost::num_vertices(network_graph));

    boost::dijkstra_shortest_paths(network_graph,start_node,boost::weight_map(boost::get(&Cable::length,network_graph))
				   .distance_map(boost::make_iterator_property_map(distances.begin(),id_map))
				   .predecessor_map(boost::make_iterator_property_map(predecessors.begin(),id_map)));

    typedef std::vector<std::string> path_t;
    path_t path;

    for(vertex_t u = predecessors[end_node]; u != end_node ; end_node =u, u=predecessors[end_node])
      {
	path.push_back(network_graph[u].name);
      }

    return path;
}

int Network::load_from_file(std::string& path)
{
  std::ifstream in(path);
  if(!read_graphviz(in, network_graph, dp, "label"))
    {
      std::cerr << "Error while reading the graph at : " << path << std::endl;
      return -1;
    }
  in.close();
  return 0;
}

void Network::save_to_file(std::string& path)
{
  std::ofstream out(path,std::ofstream::out);
  write_graphviz_dp(out, network_graph, dp, "label");
  out.close();
}

bool Network::is_connected(){

//	edge_list_t::iterator it;	
	
	return true;
}
