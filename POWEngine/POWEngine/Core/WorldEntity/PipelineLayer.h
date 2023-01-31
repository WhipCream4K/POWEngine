#pragma once

enum class PipelineLayer
{
	InputValidation,
	Update,
	PhysicsValidation,
	PostUpdate,
	IssueRenderCommand,
	Count
};
