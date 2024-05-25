import { NgModule } from "@angular/core";
import { CommonModule } from '@angular/common';
import { MessagePageComponent } from "./message-page.component";
import { SlideBarModule } from "../../components/slidebar/slidebar.module";

@NgModule({
  declarations: [MessagePageComponent],
  imports: [CommonModule, SlideBarModule],
  exports: [MessagePageComponent],
})
export class MessagePageModule {}