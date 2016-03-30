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

namespace NGTTestAutomation.Generic_app_UI
{
#pragma warning disable 0436 //(CS0436) The type 'type' in 'assembly' conflicts with the imported type 'type2' in 'assembly'. Using the type defined in 'assembly'.
    /// <summary>
    ///The AB_searchFilter recording.
    /// </summary>
    [TestModule("6e148f37-0147-4956-a96a-6875f8657779", ModuleType.Recording, 1)]
    public partial class AB_searchFilter : ITestModule
    {
        /// <summary>
        /// Holds an instance of the NGTTestAutomation.NGTTestAutomationRepository repository.
        /// </summary>
        public static NGTTestAutomation.NGTTestAutomationRepository repo = NGTTestAutomation.NGTTestAutomationRepository.Instance;

        static AB_searchFilter instance = new AB_searchFilter();

        /// <summary>
        /// Constructs a new instance.
        /// </summary>
        public AB_searchFilter()
        {
        }

        /// <summary>
        /// Gets a static instance of this recording.
        /// </summary>
        public static AB_searchFilter Instance
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

            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'Generic_app.MainWindow.FilterText_Search_filter' at Center.", repo.Generic_app.MainWindow.FilterText_Search_filterInfo, new RecordItemIndex(0));
            repo.Generic_app.MainWindow.FilterText_Search_filter.Click();
            Delay.Milliseconds(200);
            
            Report.Log(ReportLevel.Info, "Set Value", "Setting attribute Text to 'dll' on item 'Generic_app.MainWindow.FilterText_Search_filter'.", repo.Generic_app.MainWindow.FilterText_Search_filterInfo, new RecordItemIndex(1));
            repo.Generic_app.MainWindow.FilterText_Search_filter.Element.SetAttributeValue("Text", "dll");
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Keyboard", "Key 'Enter' Press.", new RecordItemIndex(2));
            Keyboard.Press(System.Windows.Forms.Keys.Return, Keyboard.DefaultScanCode, Keyboard.DefaultKeyPressTime, 1, true);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Delay", "Waiting for 500ms.", new RecordItemIndex(3));
            Delay.Duration(500, false);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating CompareImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=1646,Height=200}) on item 'Generic_app.MainWindow.AssetGrid'.", repo.Generic_app.MainWindow.AssetGridInfo, new RecordItemIndex(4));
            Validate.CompareImage(repo.Generic_app.MainWindow.AssetGridInfo, AssetGrid_Screenshot1, AssetGrid_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'Generic_app.MainWindow.CloseButton' at Center.", repo.Generic_app.MainWindow.CloseButtonInfo, new RecordItemIndex(5));
            repo.Generic_app.MainWindow.CloseButton.Click();
            Delay.Milliseconds(200);
            
            Report.Log(ReportLevel.Info, "Delay", "Waiting for 500ms.", new RecordItemIndex(6));
            Delay.Duration(500, false);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating CompareImage (Screenshot: 'Screenshot2' with region {X=0,Y=0,Width=1646,Height=204}) on item 'Generic_app.MainWindow.AssetGrid'.", repo.Generic_app.MainWindow.AssetGridInfo, new RecordItemIndex(7));
            Validate.CompareImage(repo.Generic_app.MainWindow.AssetGridInfo, AssetGrid_Screenshot2, AssetGrid_Screenshot2_Options);
            Delay.Milliseconds(0);
            
        }

#region Image Feature Data
        CompressedImage AssetGrid_Screenshot1
        { get { return repo.Generic_app.MainWindow.AssetGridInfo.GetScreenshot1(new Rectangle(0, 0, 1646, 200)); } }

        Imaging.FindOptions AssetGrid_Screenshot1_Options
        { get { return Imaging.FindOptions.Parse("0.8;EdgesSobel;0,0,1646,200;True;10000000;0ms"); } }

        CompressedImage AssetGrid_Screenshot2
        { get { return repo.Generic_app.MainWindow.AssetGridInfo.GetScreenshot2(new Rectangle(0, 0, 1646, 204)); } }

        Imaging.FindOptions AssetGrid_Screenshot2_Options
        { get { return Imaging.FindOptions.Parse("0.85;EdgesSobel;0,0,1646,204;True;10000000;0ms"); } }

#endregion
    }
#pragma warning restore 0436
}
