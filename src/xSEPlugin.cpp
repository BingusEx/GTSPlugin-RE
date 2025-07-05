using namespace std;
using namespace SKSE;
using namespace RE;

namespace {

}

SKSEPluginLoad(const LoadInterface* a_Interface) {
	return true;
}

SKSEPluginInfo(
	.Version = Plugin::ModVersion,
	.Name = Plugin::ModName,
	.Author = "Andy, BingusEx, Sermit",
	.StructCompatibility = SKSE::StructCompatibility::Independent,
	.RuntimeCompatibility = SKSE::VersionIndependence::AddressLibrary
);