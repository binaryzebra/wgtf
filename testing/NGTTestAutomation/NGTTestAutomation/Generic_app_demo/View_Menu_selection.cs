﻿///////////////////////////////////////////////////////////////////////////////
//
// This file was automatically generated by RANOREX.
// DO NOT MODIFY THIS FILE! It is regenerated by the designer.
// All your modifications will be lost!
// http://www.ranorex.com
//
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Drawing;
using System.Threading;
using WinForms = System.Windows.Forms;

using Ranorex;
using Ranorex.Core;
using Ranorex.Core.Testing;
using Ranorex.Core.Repository;

namespace NGTTestAutomation.Generic_app_demo
{
#pragma warning disable 0436 //(CS0436) The type 'type' in 'assembly' conflicts with the imported type 'type2' in 'assembly'. Using the type defined in 'assembly'.
    /// <summary>
    ///The View_Menu_selection recording.
    /// </summary>
    [TestModule("0057bfca-3540-48d9-ba3c-bbfd887e62df", ModuleType.Recording, 1)]
    public partial class View_Menu_selection : ITestModule
    {
        /// <summary>
        /// Holds an instance of the NGTTestAutomation.NGTTestAutomationRepository repository.
        /// </summary>
        public static NGTTestAutomation.NGTTestAutomationRepository repo = NGTTestAutomation.NGTTestAutomationRepository.Instance;

        static View_Menu_selection instance = new View_Menu_selection();

        /// <summary>
        /// Constructs a new instance.
        /// </summary>
        public View_Menu_selection()
        {
        }

        /// <summary>
        /// Gets a static instance of this recording.
        /// </summary>
        public static View_Menu_selection Instance
        {
            get { return instance; }
        }

#region Variables

#endregion

        /// <summary>
        /// Starts the replay of the static recording <see cref="Instance"/>.
        /// </summary>
        [System.CodeDom.Compiler.GeneratedCode("Ranorex", "5.4.5")]
        public static void Start()
        {
            TestModuleRunner.Run(Instance);
        }

        /// <summary>
        /// Performs the playback of actions in this recording.
        /// </summary>
        /// <remarks>You should not call this method directly, instead pass the module
        /// instance to the <see cref="TestModuleRunner.Run(ITestModule)"/> method
        /// that will in turn invoke this method.</remarks>
        [System.CodeDom.Compiler.GeneratedCode("Ranorex", "5.4.5")]
        void ITestModule.Run()
        {
            Mouse.DefaultMoveTime = 300;
            Keyboard.DefaultKeyPressTime = 100;
            Delay.SpeedFactor = 1.0;

            Init();

            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'Generic_app.MainWindow.View' at Center.", repo.Generic_app.MainWindow.ViewInfo, new RecordItemIndex(0));
            repo.Generic_app.MainWindow.View.Click();
            Delay.Milliseconds(200);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'Menu.View.Macros'.", repo.Menu.View.MacrosInfo, new RecordItemIndex(1));
            Validate.ContainsImage(repo.Menu.View.MacrosInfo, Macros_Screenshot1, Macros_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'Menu.View.History'.", repo.Menu.View.HistoryInfo, new RecordItemIndex(2));
            Validate.ContainsImage(repo.Menu.View.HistoryInfo, History_Screenshot1, History_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            //Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'Menu.View.TreeExpansionTest'.", repo.Menu.View.TreeExpansionTestInfo, new RecordItemIndex(3));
            //Validate.ContainsImage(repo.Menu.View.TreeExpansionTestInfo, TreeExpansionTest_Screenshot1, TreeExpansionTest_Screenshot1_Options);
            //Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'Menu.View.CustomModelInterface'.", repo.Menu.View.CustomModelInterfaceInfo, new RecordItemIndex(4));
            Validate.ContainsImage(repo.Menu.View.CustomModelInterfaceInfo, CustomModelInterface_Screenshot1, CustomModelInterface_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'Menu.View.ActiveFiltersControlTest'.", repo.Menu.View.ActiveFiltersControlTestInfo, new RecordItemIndex(5));
            Validate.ContainsImage(repo.Menu.View.ActiveFiltersControlTestInfo, ActiveFiltersControlTest_Screenshot1, ActiveFiltersControlTest_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'Menu.View.ListModelTest'.", repo.Menu.View.ListModelTestInfo, new RecordItemIndex(6));
            Validate.ContainsImage(repo.Menu.View.ListModelTestInfo, ListModelTest_Screenshot1, ListModelTest_Screenshot1_Options, "0.98");
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'Menu.View.ListFilterTest'.", repo.Menu.View.ListFilterTestInfo, new RecordItemIndex(7));
            Validate.ContainsImage(repo.Menu.View.ListFilterTestInfo, ListFilterTest_Screenshot1, ListFilterTest_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'Menu.View.TreeModelTest'.", repo.Menu.View.TreeModelTestInfo, new RecordItemIndex(8));
            Validate.ContainsImage(repo.Menu.View.TreeModelTestInfo, TreeModelTest_Screenshot1, TreeModelTest_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'Menu.View.WGContextMenuTest'.", repo.Menu.View.WGContextMenuTestInfo, new RecordItemIndex(9));
            Validate.ContainsImage(repo.Menu.View.WGContextMenuTestInfo, WGContextMenuTest_Screenshot1, WGContextMenuTest_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'Menu.View.PythonTest1'.", repo.Menu.View.PythonTest1Info, new RecordItemIndex(10));
            Validate.ContainsImage(repo.Menu.View.PythonTest1Info, PythonTest1_Screenshot1, PythonTest1_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'Menu.View.PythonTest2'.", repo.Menu.View.PythonTest2Info, new RecordItemIndex(11));
            Validate.ContainsImage(repo.Menu.View.PythonTest2Info, PythonTest2_Screenshot1, PythonTest2_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'Menu.View.PrototypeCustomPanel'.", repo.Menu.View.PrototypeCustomPanelInfo, new RecordItemIndex(12));
            Validate.ContainsImage(repo.Menu.View.PrototypeCustomPanelInfo, PrototypeCustomPanel_Screenshot1, PrototypeCustomPanel_Screenshot1_Options);
            Delay.Milliseconds(0);
            
        }

#region Image Feature Data
        CompressedImage Macros_Screenshot1
        { get { return repo.Menu.View.MacrosInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions Macros_Screenshot1_Options
        { get { return Imaging.FindOptions.Parse("0.95;EdgesSobel;0,0,0,0;True;10000000;0ms"); } }

        CompressedImage History_Screenshot1
        { get { return repo.Menu.View.HistoryInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions History_Screenshot1_Options
        { get { return Imaging.FindOptions.Parse("0.95;EdgesSobel;0,0,0,0;True;10000000;0ms"); } }

        CompressedImage TreeExpansionTest_Screenshot1
        { get { return repo.Menu.View.TreeExpansionTestInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions TreeExpansionTest_Screenshot1_Options
        { get { return Imaging.FindOptions.Parse("0.95;EdgesSobel;0,0,0,0;True;10000000;0ms"); } }

        CompressedImage CustomModelInterface_Screenshot1
        { get { return repo.Menu.View.CustomModelInterfaceInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions CustomModelInterface_Screenshot1_Options
        { get { return Imaging.FindOptions.Parse("0.95;EdgesSobel;0,0,0,0;True;10000000;0ms"); } }

        CompressedImage ActiveFiltersControlTest_Screenshot1
        { get { return repo.Menu.View.ActiveFiltersControlTestInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions ActiveFiltersControlTest_Screenshot1_Options
        { get { return Imaging.FindOptions.Parse("0.95;EdgesSobel;0,0,0,0;True;10000000;0ms"); } }

        CompressedImage ListModelTest_Screenshot1
        { get { return repo.Menu.View.ListModelTestInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions ListModelTest_Screenshot1_Options
        { get { return Imaging.FindOptions.Parse("0.95;EdgesSobel;0,0,0,0;True;10000000;0ms"); } }

        CompressedImage ListFilterTest_Screenshot1
        { get { return repo.Menu.View.ListFilterTestInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions ListFilterTest_Screenshot1_Options
        { get { return Imaging.FindOptions.Parse("0.95;EdgesSobel;0,0,0,0;True;10000000;0ms"); } }

        CompressedImage TreeModelTest_Screenshot1
        { get { return repo.Menu.View.TreeModelTestInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions TreeModelTest_Screenshot1_Options
        { get { return Imaging.FindOptions.Parse("0.95;EdgesSobel;0,0,0,0;True;10000000;0ms"); } }

        CompressedImage WGContextMenuTest_Screenshot1
        { get { return repo.Menu.View.WGContextMenuTestInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions WGContextMenuTest_Screenshot1_Options
        { get { return Imaging.FindOptions.Parse("0.95;EdgesSobel;0,0,0,0;True;10000000;0ms"); } }

        CompressedImage PythonTest1_Screenshot1
        { get { return repo.Menu.View.PythonTest1Info.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions PythonTest1_Screenshot1_Options
        { get { return Imaging.FindOptions.Parse("0.95;EdgesSobel;0,0,0,0;True;10000000;0ms"); } }

        CompressedImage PythonTest2_Screenshot1
        { get { return repo.Menu.View.PythonTest2Info.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions PythonTest2_Screenshot1_Options
        { get { return Imaging.FindOptions.Parse("0.95;EdgesSobel;0,0,0,0;True;10000000;0ms"); } }

        CompressedImage PrototypeCustomPanel_Screenshot1
        { get { return repo.Menu.View.PrototypeCustomPanelInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions PrototypeCustomPanel_Screenshot1_Options
        { get { return Imaging.FindOptions.Parse("0.95;EdgesSobel;0,0,0,0;True;10000000;0ms"); } }

#endregion
    }
#pragma warning restore 0436
}
