/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <ViewAdjustment.h> and
Class <ViewAdjustment> :
Copyright (C) 2013 Chris Drain

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

#ifndef VIEWADJUSTMENT_H_INCLUDED
#define VIEWADJUSTMENT_H_INCLUDED

#include <string>
#include <memory>
#include "d3d9.h"
#include "d3dx9.h"
#include "HMDisplayInfo.h"
#ifdef VIREIO_MATRIX_MODIFIER
#include"..\..\PluginSection\Include\Vireio_GameConfig.h"
#else
#include "Vireio.h"
#include "ProxyHelper.h"
#endif

#define LEFT_CONSTANT -1
#define RIGHT_CONSTANT 1

/**
* Class for eye and head roll adjustment matrix calculation.
* Calculates left and right view projection transform matrices.
*
* ALL MATRICES are identity matrices if worldScaleFactor in game configuration not set (==zero).
* @see ShaderConstantModification
*/
class ViewAdjustment
{
public:
#ifdef VIREIO_MATRIX_MODIFIER
	ViewAdjustment(HMDisplayInfo *displayInfo, Vireio_GameConfiguration *config);
#else
	ViewAdjustment(HMDisplayInfo *displayInfo, ProxyConfig *config);
#endif
	virtual ~ViewAdjustment();

	/*** ViewAdjustment public methods ***/
#ifdef VIREIO_MATRIX_MODIFIER
	void                      Load(Vireio_GameConfiguration& cfg);
	void                      Save(Vireio_GameConfiguration& cfg);
	Vireio_GameConfiguration* Configuration() { return config; }
#else
	void                      Load(ProxyConfig& cfg);
	void                      Save(ProxyConfig& cfg);
	ProxyConfig*              Configuration() { return config; }
#endif
	void                      UpdateProjectionMatrices(float aspectRatio, float fov_horiz);
	void                      UpdateRoll(float roll);
	void		              UpdatePosition(float yaw, float pitch, float roll, float xPosition = 0.0f, float yPosition = 0.0f, float zPosition = 0.0f);
	void                      ComputeViewTransforms();
	D3DXMATRIX                PositionMatrix();
	D3DXMATRIX                LeftAdjustmentMatrix();
	D3DXMATRIX                RightAdjustmentMatrix();
	D3DXMATRIX                LeftAdjustmentMatrixNoRoll();
	D3DXMATRIX                RightAdjustmentMatrixNoRoll();
	D3DXMATRIX                LeftView();
	D3DXMATRIX                RightView();
	D3DXMATRIX                LeftViewTransform();
	D3DXMATRIX                RightViewTransform();
	D3DXMATRIX                Projection();
	D3DXMATRIX                ProjectionInverse();
	D3DXMATRIX                RollMatrix();
	D3DXMATRIX                RollMatrixNegative();
	D3DXMATRIX                RollMatrixHalf();
	D3DXMATRIX                LeftHUDMatrix();
	D3DXMATRIX                RightHUDMatrix();
	D3DXMATRIX                LeftGUIMatrix();
	D3DXMATRIX                RightGUIMatrix();
	D3DXMATRIX                Squash();
	D3DXMATRIX                HUDDistance();
	D3DXMATRIX                LeftHUD3DDepth();
	D3DXMATRIX                RightHUD3DDepth();
	D3DXMATRIX                LeftHUD3DDepthShifted();
	D3DXMATRIX                RightHUD3DDepthShifted();
	D3DXMATRIX                LeftGUI3DDepth();
	D3DXMATRIX                RightGUI3DDepth();
	D3DXMATRIX                GatheredMatrixLeft();
	D3DXMATRIX                GatheredMatrixRight();
	D3DXMATRIX                ConvergenceOffsetLeft() {	return sMatConvergenceOffsetLeft; }
	D3DXMATRIX                ConvergenceOffsetRight() { return sMatConvergenceOffsetRight; }
	void                      GatherMatrix(D3DXMATRIX& matrixLeft, D3DXMATRIX& matrixRight);
	float                     ChangeWorldScale(float toAdd);
	float                     SetConvergence(float newConvergence);
	float                     ChangeConvergence(float toAdd);
	void                      ChangeGUISquash(float newSquash);
	void                      ChangeGUI3DDepth(float newGui3DDepth);
	void                      ChangeHUDDistance(float newHudDistance);
	void                      ChangeHUD3DDepth(float newHud3DDepth);
	void                      SetGameSpecificPositionalScaling(D3DXVECTOR3 scalingVec);
	float                     Convergence();
	float                     ConvergenceInWorldUnits();
	float                     SeparationInWorldUnits();
	float                     SeparationIPDAdjustment();
	HMDisplayInfo*            HMDInfo();

private:
#ifdef VIREIO_MATRIX_MODIFIER
	/**
	* Vireio v4.x game configuration.
	***/
	Vireio_GameConfiguration *config;
#else
	/**
	* Vireio v1.x -> v3.x game configuration.
	***/
	ProxyConfig *config;
#endif
	/**
	*
	***/
	D3DXVECTOR3 positionTransformVec;
	/**
	* Positional translation matrix
	**/
	D3DXMATRIX matPosition;
	/**
	* Projection matrix - basic with no PFOV
	***/
	D3DXMATRIX matBasicProjection;
	/**
	* Projection inverse matrix.
	***/
	D3DXMATRIX matProjectionInv;
	/**
	* The projection with adjusted FOV.
	***/
	D3DXMATRIX projectPFOV;
	/**
	* The projection with left eye convergence.
	***/
	D3DXMATRIX projectLeftConverge;
	/**
	* The projection with right eye convergence.
	***/
	D3DXMATRIX projectRightConverge;
	/**
	* The head roll matrix.
	***/
	D3DXMATRIX rollMatrix;
	/**
	* The head roll matrix. (negative)
	***/
	D3DXMATRIX rollMatrixNegative;
	/**
	* The head roll matrix. (half roll)
	***/
	D3DXMATRIX rollMatrixHalf;
	/**
	* Left matrix used to roll (if roll enabled) and shift view for ipd.
	***/
	D3DXMATRIX transformLeft;
	/**
	* Right matrix used to roll (if roll enabled) and shift view for ipd.
	***/
	D3DXMATRIX transformRight;
	/**
	* Left view projection matrix.
	***/
	D3DXMATRIX matViewProjLeft;
	/**
	* Right view projection matrix.
	***/
	D3DXMATRIX matViewProjRight;
	/**
	* Left view projection transform matrix.
	***/
	D3DXMATRIX matViewProjTransformLeft;
	/**
	* Right view projection transform matrix.
	***/
	D3DXMATRIX matViewProjTransformRight;
	/**
	* Left view projection transform matrix.
	***/
	D3DXMATRIX matViewProjTransformLeftNoRoll;
	/**
	* Right view projection transform matrix.
	***/
	D3DXMATRIX matViewProjTransformRightNoRoll;
	/**
	* Gathered matrix to be used in gathered modifications.
	***/
	D3DXMATRIX matGatheredLeft;
	/**
	* Gathered matrix to be used in gathered modifications.
	***/
	D3DXMATRIX matGatheredRight;
	/**
	* Left HUD matrix.
	***/
	D3DXMATRIX matHudLeft;
	/**
	* Right HUD matrix
	***/
	D3DXMATRIX matHudRight;
	/**
	* Left GUI matrix.
	***/
	D3DXMATRIX matGuiLeft;
	/**
	* Right GUI matrix.
	***/
	D3DXMATRIX matGuiRight;
	/**
	* Squash scaling matrix, to be used in HUD/GUI scaling matrices.
	***/
	D3DXMATRIX matSquash;
	/**
	* HUD distance matrix, to be used in HUD scaling matrices.
	***/
	D3DXMATRIX matHudDistance;
	/**
	* HUD 3d depth matrix, to be used in HUD separation matrices.
	***/
	D3DXMATRIX matLeftHud3DDepth;
	/**
	* HUD 3d depth matrix, to be used in HUD separation matrices.
	***/
	D3DXMATRIX matRightHud3DDepth;
	/**
	* HUD 3d depth matrix, to be used in HUD separation matrices.
	***/
	D3DXMATRIX matLeftHud3DDepthShifted;
	/**
	* HUD 3d depth matrix, to be used in HUD separation matrices.
	***/
	D3DXMATRIX matRightHud3DDepthShifted;
	/**
	* HUD 3d depth matrix, to be used in HUD separation matrices.
	***/
	D3DXMATRIX matLeftGui3DDepth;
	/**
	* HUD 3d depth matrix, to be used in HUD separation matrices.
	***/
	D3DXMATRIX matRightGui3DDepth;
	/**
	* Convergence offset left. (only translation)
	***/
	D3DXMATRIX sMatConvergenceOffsetLeft;
	/**
	* Convergence offset right. (only translation)
	***/
	D3DXMATRIX sMatConvergenceOffsetRight;
	/**
	* Used to scale the positional movement, seems x/y/z are not equal
	*/
	D3DXVECTOR3 gameScaleVec;
	/**
	* Head mounted display info.
	***/
	HMDisplayInfo* hmdInfo;
	/**
	* Amount of actual roll (in radians)
	*/
	float m_roll;
	/**
	* Interpupillary distance.
	* As provided from Oculus Configuration Utility (or set in the "user.xml" file).
	***/
	float ipd;
	/**
	* Convergence.
	* Left/Rigth offset adjustment. In millimeters.
	***/
	float convergence;
	/**
	* The amount of squashing GUI shader constants.
	* 1.0 == full render
	***/
	float squash;
	/**
	* The 3d depth of the GUI.
	***/
	float gui3DDepth;
	/**
	* The distance of the HUD.
	***/
	float hudDistance;
	/**
	* The 3d depth of the HUD.
	***/
	float hud3DDepth;
};
#endif