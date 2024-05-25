import { NgModule } from "@angular/core";
import { CommonModule } from '@angular/common';
import { HomePageComponent } from "./home-page.component";
import { HomePageRoutingModule } from "./home-page.routes";
import { SlideBarModule } from "../../components/slidebar/slidebar.module";

@NgModule({
  declarations: [HomePageComponent],
  imports: [CommonModule, HomePageRoutingModule, SlideBarModule],
  exports: [HomePageComponent],
})
export class HomePageModule {}