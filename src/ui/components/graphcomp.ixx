export module gui:graphcomp;

import std;

import <imgui.h>;
import <implot/implot.h>;

import :controller;
import imguiext;
import scrollingbuffer;
import simulation;

export void GuiController::renderGraphs(const SimulationData& data)
{
	static constexpr int size = 144 * 10;

	static ScrollingBuffer<size> ts, xs, vs, as, ws, fs, gs, hs, traj;
	xs.addPoint(data.t, data.position);
	vs.addPoint(data.t, data.velocity);
	as.addPoint(data.t, data.acceleration);
	ws.addPoint(data.t, data.w);
	fs.addPoint(data.t, data.f);
	gs.addPoint(data.t, data.g);
	hs.addPoint(data.t, data.h);
	traj.addPoint(data.position, data.velocity);

	static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

	std::string xStr = std::format("x = {}", data.position);
	std::string vStr = std::format("v = {}", data.velocity);
	std::string aStr = std::format("a = {}", data.acceleration);
	std::string wStr = std::format("w = {}", data.w);
	std::string fStr = std::format("f = {}", data.f);
	std::string gStr = std::format("g = {}", data.g);
	std::string hStr = std::format("h = {}", data.h);

	ImGui::Text(xStr.c_str());
	ImGui::Text(vStr.c_str());
	ImGui::Text(aStr.c_str());

	ImGui::NewLine();
	ImGui::Text(wStr.c_str());
	ImGui::Text(fStr.c_str());
	ImGui::Text(gStr.c_str());
	ImGui::Text(hStr.c_str());

	static float history = 5.0f;
	ImGui::NewLine();
	ImGui::SliderFloat("History", &history, 1, 10, "%.1f s");

	if (ImPlot::BeginPlot("x(t), v(t), a(t)", ImVec2(-1, 300)))
	{
		ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
		ImPlot::SetupAxisLimits(ImAxis_X1, data.t - history/2, data.t + history / 2, ImGuiCond_Always);
		ImPlot::SetupAxisLimits(ImAxis_Y1, -1.0f, 1.0f);
		ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
		ImPlot::PlotLine("x(t)", &xs.data[0].x, &xs.data[0].y, xs.data.size(), 0, xs.offset, 2 * sizeof(float));
		ImPlot::PlotLine("v(t)", &vs.data[0].x, &vs.data[0].y, vs.data.size(), 0, vs.offset, 2 * sizeof(float));
		ImPlot::PlotLine("a(t)", &as.data[0].x, &as.data[0].y, as.data.size(), 0, as.offset, 2 * sizeof(float));
		ImPlot::EndPlot();
	}

	if (ImPlot::BeginPlot("w(t), f(t), g(t), h(t)", ImVec2(-1, 300)))
	{
		ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
		ImPlot::SetupAxisLimits(ImAxis_X1, data.t - history / 2, data.t + history / 2, ImGuiCond_Always);
		ImPlot::SetupAxisLimits(ImAxis_Y1, -1.0f, 1.0f);
		ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
		ImPlot::PlotLine("w(t)", &ws.data[0].x, &ws.data[0].y, ws.data.size(), 0, ws.offset, 2 * sizeof(float));
		ImPlot::PlotLine("f(t)", &fs.data[0].x, &fs.data[0].y, fs.data.size(), 0, fs.offset, 2 * sizeof(float));
		ImPlot::PlotLine("g(t)", &gs.data[0].x, &gs.data[0].y, gs.data.size(), 0, gs.offset, 2 * sizeof(float));
		ImPlot::PlotLine("h(t)", &hs.data[0].x, &hs.data[0].y, hs.data.size(), 0, hs.offset, 2 * sizeof(float));
		ImPlot::EndPlot();
	}

	if (ImPlot::BeginPlot("Trajectory", ImVec2(-1, 300)))
	{
		ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
		ImPlot::SetupAxisLimits(ImAxis_X1, -1.0f, 1.0f, ImGuiCond_Always);
		ImPlot::SetupAxisLimits(ImAxis_Y1, -1.0f, 1.0f);
		ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
		ImPlot::PlotLine("(x(t), v(t))", &traj.data[0].x, &traj.data[0].y, traj.data.size(), 0, traj.offset, 2 * sizeof(float));
		ImPlot::EndPlot();
	}
}