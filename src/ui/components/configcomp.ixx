export module gui:configcomp;

import std;

import <imgui.h>;

import :controller;
import imguiext;
import simulation;

bool inputFunctionArgs(FunctionType selected, float& A, float& omega, float& phi, const std::string& prefix)
{
	std::string ANameStr = std::format("A##{}A", prefix);
	std::string omegaNameStr = std::format("omega##{}omega", prefix);
	std::string phiNameStr = std::format("phi##{}phi", prefix);
	const char* AName = ANameStr.c_str();
	const char* omegaName = omegaNameStr.c_str();
	const char* phiName = phiNameStr.c_str();

	switch (selected)
	{
	case FunctionType::Constant:
		return ext::InputClampedFloat(AName, &A, -1.0f, 1.0f);
	case FunctionType::Step:
		return ext::InputClampedFloat(AName, &A, -1.0f, 1.0f);
	case FunctionType::Sgn:
		return ext::InputClampedFloat(AName, &A, -1.0f, 1.0f) |
		ext::InputClampedFloat(omegaName, &omega, 0.0f, 10.0f) |
		ext::InputClampedFloat(phiName, &phi, -2 * std::numbers::pi_v<float>, 2 * std::numbers::pi_v<float>);
		break;
	case FunctionType::Sin:
		return ext::InputClampedFloat(AName, &A, -1.0f, 1.0f) |
		ext::InputClampedFloat(omegaName, &omega, 0.0f, 10.0f) |
		ext::InputClampedFloat(phiName, &phi, -2 * std::numbers::pi_v<float>, 2 * std::numbers::pi_v<float>);
	default:
		return false;
	}
}

export void GuiController::renderConfig()
{
	ext::InputClampedFloat("x0", &data.position, -1.0f, 1.0f);
	ext::InputClampedFloat("v0", &data.velocity, -1.0f, 1.0f);
	ImGui::NewLine();

	ext::InputClampedDouble("dt", &config.dt, 0.0001, 1.0);
	ext::InputClampedFloat("m", &config.m, 0.0f, 10.0f);
	ext::InputClampedFloat("k", &config.k, 0.0f, 10.0f);
	ext::InputClampedFloat("c", &config.c, 0.0f, 10.0f);

	static FunctionType selectedWFunc = FunctionType::Constant;
	static FunctionType selectedHFunc = FunctionType::Constant;
	static const char* w_items[]{ "const w", "step w", "sgn w", "sin w"};
	static const char* h_items[]{ "const h", "step h", "sgn h", "sin h" };
	bool wfunctionChanged = false;
	bool hFunctionChanged = false;

	// w(t)
	wfunctionChanged |= ImGui::Combo("w(t)", reinterpret_cast<int*>(&selectedWFunc), w_items, IM_ARRAYSIZE(w_items));
	static float w_A = 0.0f;
	static float w_omega = 0.0f;
	static float w_phi = 0.0f;
	wfunctionChanged |= inputFunctionArgs(selectedWFunc, w_A, w_omega, w_phi, "w");

	// h(t)
	hFunctionChanged |= ImGui::Combo("h(t)", reinterpret_cast<int*>(&selectedHFunc), h_items, IM_ARRAYSIZE(h_items));
	static float h_A = 0.0f;
	static float h_omega = 0.0f;
	static float h_phi = 0.0f;
	hFunctionChanged |= inputFunctionArgs(selectedHFunc, h_A, h_omega, h_phi, "h");
	
	if (wfunctionChanged)
		config.w = FunctionFactory::getFunction(selectedWFunc, w_A, w_omega, w_phi);
	if (hFunctionChanged)
		config.h = FunctionFactory::getFunction(selectedHFunc, h_A, h_omega, h_phi);

	if (ImGui::Button("Apply"))
		simulation.setConfig(config);

	if (ImGui::Button("Restart"))
	{
		ImGui::SameLine();
		simulation.setData(data);
	}		
}