// WistoneGUI.cpp : main project file.

#include "stdafx.h"
#include "GUI.h"

namespace WistoneGUI
{
	using namespace System;
	using namespace System::Windows::Forms;

	[STAThread]
	int main(array<System::String ^> ^args)
	{
		// Enabling visual effects before any controls are created
		Application::EnableVisualStyles(); // YL 2.5 copied from UpdatedApplication

		// Use GDI graphics class for text rendering
		Application::SetCompatibleTextRenderingDefault(false); // YL 2.5 copied from UpdatedApplication

		// Create the GUI console
		Application::Run(gcnew GUI());

		return 0;
	}
}